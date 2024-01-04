import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime
import locale

locale.setlocale(locale.LC_ALL, "en_US.UTF-8")

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    userID = session["user_id"]
    user = db.execute("SELECT * FROM users WHERE id = ?", userID)  # gets our user
    cash = round(user[0]["cash"], 2)  # will pass this in, make sure it's not weird

    symbols = db.execute(
        "SELECT symbol FROM stocks WHERE username = ?", user[0]["username"]
    )  # gets all the user's purchased companies

    table = (
        []
    )  # empty table that we will populate with symbol, price of symbol, and shares purchased

    total = cash
    # how much moners they have

    for row in symbols:
        symbol = row["symbol"]
        shares = db.execute(
            "SELECT shares FROM stocks WHERE username = ? AND symbol = ?",
            user[0]["username"],
            symbol,
        )  # sums up all of the user's shares of that symbol
        shares = shares[0]["shares"]
        lookupValue = lookup(symbol)
        price = lookupValue["price"]  # gets the current price of that symbol
        value = shares * price  # multiplies the two together to get the total value
        total += value

        price = locale.currency(price, grouping=True)
        value = locale.currency(value, grouping=True)  # sets these two to currency form

        table.append(
            {"symbol": symbol, "shares": shares, "price": price, "value": value}
        )

    total = locale.currency(total, grouping=True)
    cash = locale.currency(cash, grouping=True)  # sets these two to currency form

    return render_template("index.html", cash=cash, table=table, total=total)


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    if request.method == "POST":
        cash = int(request.form.get("cash"))
        if not cash:
            return apology("no input", 403)
        if cash <= 0:
            return apology("input not a positive number", 403)

        userID = session["user_id"]
        user = db.execute("SELECT * FROM users WHERE id = ?", userID)  # gets our user
        username = user[0]["username"]

        currentCash = db.execute("SELECT cash FROM users WHERE username = ?", username)
        currentCash = currentCash[0]["cash"]  # gets how much money the user has

        db.execute(
            "UPDATE users SET cash = ? WHERE username = ?", currentCash + cash, username
        )  # adds the requested cash to their account

        return redirect("/")

    else:
        return render_template("addcash.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        if not symbol or not lookup(symbol):
            return apology("must provide valid symbol", 400)

        try:
            shares = int(request.form.get("shares"))
            if shares < 1:
                return apology(
                    "must provide 1 or more shares", 400
                )  # if in post, ensure symbol exists and shares are 1 or higher
        except ValueError:
            return apology("must provide a positive integer", 400)

        stock = lookup(symbol)
        price = stock["price"]  # gets the price of the share

        userID = session["user_id"]

        user = db.execute("SELECT * FROM users WHERE id = ?", userID)  # gets our user

        cash = user[0]["cash"]
        username = user[0]["username"]

        totalCost = price * shares

        if totalCost > cash:
            return apology(
                "insufficient funds for transaction"
            )  # if they don't have enough, tell them

        db.execute(
            "UPDATE users SET cash = ? WHERE username = ?",
            round(cash - totalCost, 2),
            username,
        )  # if they have enough money, subtract total cost from their cash

        db.execute(
            "INSERT INTO transactions (username, type, symbol, price, shares, datetime) VALUES (?, ?, ?, ?, ?, ?)",
            username,
            "Buy",
            symbol,
            price,
            shares,
            datetime.now(),
        )

        stock = db.execute(
            "SELECT symbol FROM stocks WHERE username = ? AND symbol = ?",
            username,
            symbol,
        )

        if len(stock) == 0:
            db.execute(
                "INSERT INTO stocks (username, symbol, shares) VALUES (?, ?, ?)",
                username,
                symbol,
                shares,
            )
        else:
            currentShares = db.execute(
                "SELECT shares FROM stocks username = ? AND symbol = ?",
                username,
                symbol,
            )
            currentShares = currentShares[0]["shares"]
            db.execute(
                "UPDATE stocks SET shares = ? WHERE username = ? AND symbol = ?",
                currentShares + shares,
                username,
                symbol,
            )

        #  inserts the transaction's who, how much (price and shares), and time
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    userID = session["user_id"]
    user = db.execute("SELECT * FROM users WHERE id = ?", userID)  # gets our user
    username = user[0]["username"]

    transactions = db.execute("SELECT * FROM transactions WHERE username = ?", username)

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        stock = lookup(request.form.get("symbol").upper())
        if stock == None:
            return apology("stock does not exist", 400)

        price = locale.currency(stock["price"], grouping=True)
        stock = stock["symbol"]
        return render_template("quoted.html", stock=stock, price=price)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password was confirmed
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        if request.form.get("password") != request.form.get("confirmation"):
            return apology("password and confirmation do not match", 400)

        usernames = db.execute("SELECT username FROM users")

        temp = []
        for row in usernames:
            temp.append(row["username"])

        if (
            request.form.get("username") in temp
        ):  # if there is already a username matching in database
            return apology("username already exists", 400)

        db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)",
            request.form.get("username"),
            generate_password_hash(request.form.get("password")),
        )

        # redirect user to home after adding them
        return redirect("/")

    else:
        # Get their username, password, and confirm password
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    userID = session["user_id"]
    user = db.execute("SELECT * FROM users WHERE id = ?", userID)
    username = user[0]["username"]  # gets our user

    symbols = db.execute(
        "SELECT symbol FROM stocks WHERE username = ?", username
    )  # gets their owned stocks

    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology(
                "no input", 400
            )  # if not an appropriate symbol, then say is invalid

        temp = []

        for row in symbols:
            temp.append(row["symbol"])  # filling a list with symbols to check it

        if symbol in temp:
            shares = int(request.form.get("shares"))
            owned = db.execute(
                "SELECT shares FROM stocks WHERE username = ? AND symbol = ?",
                username,
                symbol,
            )
            owned = owned[0]["shares"]

            if not shares:
                return apology("no input", 400)
            elif shares < 1:
                return apology("non-positive input", 400)
            elif shares > owned:
                return apology(
                    "too many shares", 400
                )  # if not an appropriate stock amount, say is invalid

            lookupValue = lookup(symbol)
            price = lookupValue["price"]  # gets the current price of that symbol
            value = shares * price  # multiplies the two together to get the total value

            currentCash = db.execute(
                "SELECT cash FROM users WHERE username = ?", username
            )
            currentCash = currentCash[0]["cash"]  # gets how much money the user has

            db.execute(
                "UPDATE users SET cash = ? WHERE username = ?",
                currentCash + value,
                username,
            )  # adds the sold stocks back to their cash

            db.execute(
                "INSERT INTO transactions (username, type, symbol, price, shares, datetime) VALUES (?, ?, ?, ?, ?, ?)",
                username,
                "Sell",
                symbol,
                price,
                shares,
                datetime.now(),
            )
            # records transaction

            if shares == owned:
                db.execute(
                    "DELETE FROM stocks WHERE username = ? AND symbol = ?",
                    username,
                    symbol,
                )
            else:
                db.execute(
                    "UPDATE stocks SET shares = ? WHERE username = ? AND symbol = ?",
                    owned - shares,
                    username,
                    symbol,
                )

            return redirect("/")

        else:
            return apology("chosen stock is not owned", 400)

    else:
        return render_template("sell.html", symbols=symbols)  # our symbols for the user
