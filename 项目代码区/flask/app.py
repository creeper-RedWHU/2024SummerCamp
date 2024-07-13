from flask import Flask, request, jsonify
from GetData import fetchData,GetDataByHours
from DeepLearning import run_prediction
from config import Config
import pymysql

app = Flask(__name__)

@app.route('/')
def hello_world():
    return '当你看到这条消息，说明接口没崩溃。喜欢吃柚子~~'

@app.route('/register', methods=['POST'])
def register():
    data = request.json
    account = data.get('account')
    password = data.get('password')
    identity = 1  # 只能注册普通用户

    if not account or not password:
        return jsonify({'success': False, 'message': 'Account and password required'}), 400

    connection = pymysql.connect(
        host=Config.MYSQL_HOST,
        user=Config.MYSQL_USER,
        password=Config.MYSQL_PASSWORD,
        database=Config.MYSQL_DB,
        cursorclass=pymysql.cursors.DictCursor
    )
    with connection.cursor() as cursor:
        cursor.execute("INSERT INTO haolins (account, password, identity) VALUES (%s, %s, %s)", (account, password, identity))
        connection.commit()

    return jsonify({'success': True, 'message': 'User registered successfully'})

@app.route('/login', methods=['POST'])
def login():
    data = request.json
    account = data.get('account')
    password = data.get('password')

    if not account or not password:
        return jsonify({'success': False, 'identity': None}), 400

    connection =  pymysql.connect(
        host=Config.MYSQL_HOST,
        user=Config.MYSQL_USER,
        password=Config.MYSQL_PASSWORD,
        database=Config.MYSQL_DB,
        cursorclass=pymysql.cursors.DictCursor
    )
    with connection.cursor() as cursor:
        cursor.execute("SELECT * FROM haolins WHERE account = %s AND password = %s", (account, password))
        haolins = cursor.fetchone()

    if haolins:
        return jsonify({'success': True, 'identity': haolins['identity']})
    else:
        return jsonify({'success': False, 'identity': None})

@app.route('/fetch_weather', methods=['POST'])
def fetch_weather():
    # 从请求参数中获取城市、开始年、开始月、结束年、结束月
    data = request.get_json()
    city = data.get('city')
    start_year = int(data.get('start_year'))
    start_month = int(data.get('start_month'))
    end_year = int(data.get('end_year'))
    end_month = int(data.get('end_month'))

    # 调用fetchData函数获取数据
    fetchData(city, start_year, end_year, start_month, end_month)

    # 返回成功响应
    return jsonify({'success': True, 'message': 'Data fetched successfully'})

@app.route('/today_weather', methods=['POST'])
def get_today_weather():
    data = request.get_json()
    city = data.get('city')

    GetDataByHours(city)

    #返回数据
    return jsonify({'success': True, 'message': 'successfully'})

@app.route('/ml', methods=['POST'])
def ml():
    data = request.json
    city = data.get('city')
    year = int(data.get('year'))
    month = int(data.get('month'))
    day = int(data.get('day'))
    predictions = run_prediction(city, year, month, day)
    return jsonify({'predictions': predictions.tolist()})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
