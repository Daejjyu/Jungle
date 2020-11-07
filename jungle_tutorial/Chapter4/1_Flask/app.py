# 파일 이름은 아무렇게나 해도 상관없지만, 통상적으로 flask 서버를 돌리는 파일은 app.py라고 이름 짓습니다.
# Flask 서버를 만들 때는 항상 프로젝트 폴더 안에 static, templates 폴더와 app.py를 먼저 만들고 시작해주세요.
# templates 폴더는 HTML 파일을 담아두고 불러오는 역할을 합니다.
# static 폴더는 html 파일 외에 이미지, css파일과 같은 파일을 담아두는 역할을 합니다.
# venv는 가상환경에 대한 설정 정보가 담긴 폴더로, 안의 내용을 바꾸지 않는 것이 좋기 때문에 없는 것처럼 생각하는 것이 좋습니다.
# 터미널 창을 클릭하시고, ctrl + c 을 누르시면 서버를 종료할 수 있습니다. 맥에서도 Cmd가 아니라 Ctrl!
#################################################
# flask 시작 코드
#################################################
from flask import Flask, render_template  # render_template은 html을 불러오는 내장함수

app = Flask(__name__)


@app.route('/')
def home():
    return render_template('index.html')


#################################################
# Flask 기초: URL 나누기
# url 별로 함수명이 같거나, route('/')내의 주소가 같으면 안돼요!
#################################################
@app.route('/mypage')
def mypage():
    return 'This is My Page!'


if __name__ == '__main__':
    app.run('0.0.0.0', port=5000, debug=True)
