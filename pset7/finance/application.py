from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():

    result = db.execute("""SELECT
        symbol, sum(number_shares) as shares
        FROM transactions
        join users on users.id=transactions.id_user
        join symbols on symbols.id=transactions.id_symbol
        where users.id = :id
        group by symbols.id""", id=session["user_id"])
    rows = []
    total = 0
    for row in result:
        d = {}
        q = lookup(row["symbol"])
        d["symbol"] = row["symbol"]
        d["shares"] = row["shares"]
        d["name"] = q["name"]
        d["price"] = q["price"]
        d["total"] = "%.2f" % (q["price"] * d["shares"])
        total += float(d["total"])
        rows.append(d)

    result = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    print(float(result[0]["cash"]) + total)
    return render_template("index.html",
                           cash = "%.2f" % (float(result[0]["cash"])),
                           total = "%.2f" % (float(result[0]["cash"]) + total),
                           rows = rows)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    if request.method == "GET":
        return render_template("buy-form.html")
    elif request.method == "POST" :
        # verify if the inputs are empty
        if not request.form["symbol"]:
            return apology("must provide a symbol")
        if not request.form["shares"]:
            return apology("must provide a number of shares")

        # symbols
        q = lookup(request.form["symbol"].lower())
        if q == None:
            return apology("Invalid Symbol")
        result = db.execute("SELECT id FROM symbols WHERE symbol = :symbol",
                            symbol=q["symbol"])
        if len(result) != 1:
            db.execute("INSERT INTO symbols(symbol) values (:symbol)",
                        symbol=q["symbol"])
            result = db.execute("SELECT id FROM symbols WHERE symbol = :symbol",
                                symbol=q["symbol"])
        symbol = result[0]["id"]


        # users
        result = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        price = int(request.form["shares"]) * float(q["price"])
        if result[0]["cash"] < price:
            return apology("You don't have money")
        db.execute("UPDATE users SET cash = cash - :price WHERE id = :id",
                    price=price, id=session["user_id"])

        #transactions
        db.execute("""
                    INSERT INTO transactions(id_user,id_symbol,number_shares,price,date) values
                    (:id,:symbol,:shares,:price,datetime('now'))""",
                   id=session["user_id"],
                   symbol=symbol,
                   shares=request.form["shares"],
                   price=q["price"])
        return redirect(url_for(("index")))

    return apology("error")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    result = db.execute("""
                        select
                        symbol, number_shares as shares, price, date
                        from transactions
                        join symbols on
                        symbols.id=transactions.id_symbol
                        where id_user = :id""", id=session["user_id"])
    return render_template("history.html", rows=result)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    elif request.method == "POST":
        if not request.form["symbol"]:
            return apology("must provide a symbol")
        else:
            q = lookup(request.form["symbol"].lower())
            if q == None:
                return apology("Invalid Symbol")
            else:
                return render_template("quote_show.html", quote=q)

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "POST":
        if not request.form['username']:
            return apology("Missing username!")
        if not request.form['password']:
            return apology("Missing password!")
        if not request.form['password-confirm']:
            return apology("Missing password confirm!")
        if request.form['password'] != request.form['password-confirm']:
            return apology("Password and Password confirm don't macth.")
        password = pwd_context.hash(request.form['password'])

        result = db.execute(
            "INSERT INTO users (username, hash) VALUES(:username, :hash)",
            username=request.form.get('username'),
            hash=password)
        if not result:
            return apology("This user is already registered.")
        result = db.execute(
            "SELECT id FROM users WHERE username = :username",
            username=request.form.get('username'))

        session["user_id"] = result[0]["id"]

        return redirect(url_for("index"))
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "GET":
        result = db.execute("""
        SELECT
        id_symbol as id, symbol
        FROM transactions
        join symbols on symbols.id=transactions.id_symbol
        where id_user = :id
        group by symbols.id""", id = session["user_id"])
        return render_template("sell-form.html", symbols=result)
    if request.method == "POST":
        if not request.form["symbol"]:
            return apology("must provide a symbol")
        if not request.form["shares"]:
            return apology("must provide a number of shares")

        result = db.execute("""
        select sum(number_shares) as shares from users
        join transactions on users.id = transactions.id_user
        where users.id = :user and id_symbol = :symbol
        group by id_symbol
        """, user=session["user_id"], symbol=request.form["symbol"])
        shares = result[0]["shares"]
        print(shares)
        print(request.form["shares"])
        if int(shares) < int(request.form["shares"]):
            return apology("you don't have this number of shares")

        result = db.execute("select symbol from symbols where id = :id", id = request.form["symbol"])
        s = result[0]["symbol"]
        q = lookup(s)

        db.execute("""
                   insert into transactions(id_user,id_symbol,number_shares,price,date) values
                   (:user,:symbol,-:shares,:price,datetime('now'))""",
                   user=session["user_id"],
                   symbol=request.form["symbol"],
                   shares=request.form["shares"],
                   price=q["price"])

        price = float(q["price"]) * int(request.form["shares"])
        db.execute("UPDATE users SET cash = cash + :price WHERE id = :id",
                    price=price, id=session["user_id"])
        return redirect("/")
    return apology("error")

@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    if request.method == "GET":
        return render_template("password.html")
    if request.method == "POST":
        if not request.form["atual"]:
            return apology("must provide the atual password")
        if not request.form["pass"]:
            return apology("must provide a new password")
        if not request.form["pass-confirm"]:
            return apology("must provide the password confirm")

        result = db.execute("select hash from users where id =:id",
                            id = session["user_id"])
        if not pwd_context.verify(request.form.get("atual"), result[0]["hash"]):
            return apology("invalid pass")
        if request.form['pass'] != request.form['pass-confirm']:
            return apology("password confirm don't macth")
        password=pwd_context.hash(request.form['pass'])
        db.execute("update users set hash = :hash",
                   hash=password)
        return render_template("pass.html")
    return apology("error")