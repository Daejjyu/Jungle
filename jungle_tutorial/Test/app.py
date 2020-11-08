## POST : 클라이언트가 유저의 입력값 전송 -> 서버 웹스크래핑 db에 게시
## GET : db에서 조회해서 -> 클라이언트 GET해서 card추가

from flask import Flask, render_template, jsonify, request
import requests
from bs4 import BeautifulSoup
from pymongo import MongoClient  # pymongo를 임포트 하기(패키지 인스톨 먼저 해야겠죠?)

app = Flask(__name__)

client = MongoClient('localhost', 27017)  # mongoDB는 27017 포트로 돌아갑니다.

# client = MongoClient('mongodb://test:test@13.209.7.191', 27017)  # mongoDB는 27017 포트로 돌아갑니다.
db = client.dbsparta  # 'dbsparta'라는 이름의 db를 만들거나 사용합니다.


@app.route('/')
def home():
    return render_template('index.html')


@app.route('/memo', methods=['POST'])
def post_article():
    # 1. 클라이언트로부터 데이터를 받기
    title_receive = request.form['title_give']  # 클라이언트로부터 url을 받는 부분
    text_receive = request.form['text_give']  # 클라이언트로부터 comment를 받는 부분

    article = {'title': title_receive, 'text': text_receive}

    # 3. mongoDB에 데이터를 넣기
    db.articles.insert_one(article)

    return jsonify({'result': 'success'})


@app.route('/delete', methods=['POST'])
def delete_article():
    title_receive = request.form['title_give']  # 클라이언트로부터 url을 받는 부분
    text_receive = request.form['text_give']  # 클라이언트로부터 comment를 받는 부분

    db.users.delete_one({'name': title_receive})
    article = {'title': title_receive, 'text': text_receive}

    db.articles.delete_one(article)

    return jsonify({'result': 'success'})


@app.route('/update', methods=['POST'])
def update_article():
    # 1. 클라이언트로부터 데이터를 받기
    title_receive = request.form['title_give']  # 클라이언트로부터 url을 받는 부분
    text_receive = request.form['text_give']  # 클라이언트로부터 comment를 받는 부분
    temp_title_receive = request.form['titleUpdate_give'];
    temp_text_receive = request.form['textUpdate_give'];
    print(1, title_receive)
    print(2, text_receive)
    print(3, temp_title_receive)
    print(4, temp_text_receive)

    db.users.update_one({'title': title_receive}, {'$set': {'text': temp_title_receive}})
    db.users.update_one({'text': text_receive}, {'$set': {'title': temp_text_receive}})

    return jsonify({'result': 'success'})


@app.route('/memo', methods=['GET'])  # 브라우저에서 주소창에 엔터를 치는 것이 GET 요청과 같다는 사실, 기억하시죠??
def read_articles():
    # 1. mongoDB에서 _id 값을 제외한 모든 데이터 조회해오기 (Read)
    result = list(db.articles.find({}, {'_id': 0}))
    # 2. articles라는 키 값으로 article 정보 보내주기
    return jsonify({'result': 'success', 'articles': result})


if __name__ == '__main__':
    app.run('0.0.0.0', port=5000, debug=True)
