from flask import Flask, request, jsonify
from db import get_db_connection

app = Flask(__name__)

@app.route('/register', methods=['POST'])
def register():
    data = request.json
    account = data.get('account')
    password = data.get('password')
    identity = 1  # 只能注册普通用户

    if not account or not password:
        return jsonify({'success': False, 'message': 'Account and password required'}), 400

    connection = get_db_connection()
    with connection.cursor() as cursor:
        cursor.execute("INSERT INTO users (account, password, identity) VALUES (%s, %s, %s)", (account, password, identity))
        connection.commit()

    return jsonify({'success': True, 'message': 'Users registered successfully'})

@app.route('/login', methods=['POST'])
def login():
    data = request.json
    account = data.get('account')
    password = data.get('password')

    if not account or not password:
        return jsonify({'success': False, 'identity': None}), 400

    connection = get_db_connection()
    with connection.cursor() as cursor:
        cursor.execute("SELECT * FROM users WHERE account = %s AND password = %s", (account, password))
        users = cursor.fetchone()

    if users:
        return jsonify({'success': True, 'identity': users['identity']})
    else:
        return jsonify({'success': False, 'identity': None})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
