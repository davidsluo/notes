from flask import Flask, render_template, request, redirect, flash
from werkzeug.utils import secure_filename
import random
import os
import subprocess
import io

UPLOAD_PATH = './uploads'

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_PATH
app.secret_key = 'asdlf;kjasdl;kfj;aslkdjfl;k'


@app.route('/', methods=['GET', 'POST'])
def upload():
    if request.method == 'POST':
        if 'file' not in request.files:
            flash('No file selected.')
            return redirect(request.url)
        file = request.files['file']
        if file.filename == '':
            flash('No file selected.')
            return redirect(request.url)
        if file:
            filename = secure_filename(file.filename)
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))

            result = subprocess.check_output("python compile.py".split())
            flash('Uploaded')
            return render_template('index.html', result=result.decode())
    else:
        flash('Select a file for grading.')
        return render_template('index.html')


if __name__ == "__main__":
    # app.run(host="0.0.0.0", debug=True)
    app.run(host="0.0.0.0", debug=False)
