from flask import Flask, jsonify

app = Flask(__name__)

def print_hello_world():
    return "Hello, World!"

@app.route('/hello', methods=['GET'])
def hello():
    message = print_hello_world()
    return jsonify(message=message)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
