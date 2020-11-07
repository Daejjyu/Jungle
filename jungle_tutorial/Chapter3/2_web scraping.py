import requests
from bs4 import BeautifulSoup

# 타겟 URL을 읽어서 HTML를 받아오고,
headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.86 Safari/537.36'}
data = requests.get('https://movie.naver.com/movie/sdb/rank/rmovie.nhn?sel=pnt&date=20200303', headers=headers)

# HTML을 BeautifulSoup이라는 라이브러리를 활용해 검색하기 용이한 상태로 만듦
# soup이라는 변수에 "파싱 용이해진 html"이 담긴 상태가 됨
# 이제 코딩을 통해 필요한 부분을 추출하면 된다.
soup = BeautifulSoup(data.text, 'html.parser')
# print(soup)  # HTML을 받아온 것을 확인할 수 있다.

#############################
# (입맛에 맞게 코딩)
#############################


# select를 이용해서, tr들을 불러오기
# 개발자도구 Elements 탭에서 요소를 우클릭한 후 Copy > Copy selector를 해서 선택자를 얻을 수 있습니다.
movies = soup.select('#old_content > table > tbody > tr')
print(len(movies))

# movies (tr들)의 반복문을 돌리기
for movie in movies:
    a_tag = movie.select_one('td.title>div>a')
    if a_tag is not None:
        print(a_tag)

############################################################################
# soup.select('태그명')
# soup.select('.클래스명')
# soup.select('#아이디명')
#
# soup.select('상위태그명 > 하위태그명 > 하위태그명')
# soup.select('상위태그명.클래스명 > 하위태그명.클래스명')
#
# # 태그와 속성값으로 찾는 방법
# soup.select('태그명[속성="값"]')
#
# # 한 개만 가져오고 싶은 경우
# soup.select_one('위와 동일')
############################################################################


# Q.제목 뿐만 아니라 순위, 별점도 같이 스크래핑해봅시다!
i = 0
for movie in movies:
    a_tag = movie.select_one('td.title>div>a')
    rate_tag = movie.select_one('td.point')
    if a_tag is not None:
        i += 1
        if (i < 10):
            print('0' + str(i), '', end='')
        else:
            print(i, '', end='')
        print(a_tag.text, '', end='')
    if rate_tag is not None:
        print(rate_tag.text)
