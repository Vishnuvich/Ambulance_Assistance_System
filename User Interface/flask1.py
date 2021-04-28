from flask import Flask, redirect, render_template, url_for, request
import random


app = Flask(__name__)


@app.route("/")
def home():

    templateData = {
        'Temp': '37 C',
        'Testdata': 'Hi'}
    return render_template("index.html", **templateData)


@app.route('/postmethod', methods = ['GET','POST'])
def get_post_javascript_data():
    jsdata = request.args.get('javascript_data')
    print(jsdata)
    return "Hi"
        
 


if __name__ == "__main__":
    app.run(debug=True)
