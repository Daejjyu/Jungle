#######################################################
# DB연결하기 & 데이터 넣기
#######################################################
from pymongo import MongoClient  # pymongo를 임포트 하기(패키지 인스톨 먼저 해야겠죠?)

client = MongoClient('localhost', 27017)  # mongoDB는 27017 포트로 돌아갑니다.
db = client.dbsparta  # 'dbsparta'라는 이름의 db를 만듭니다.

# MongoDB에 insert 하기

# 'users'라는 collection에 {'name':'bobby','age':21}를 넣습니다.
db.users.insert_one({'name': 'bobby', 'age': 21})
db.users.insert_one({'name': 'kay', 'age': 27})
db.users.insert_one({'name': 'john', 'age': 30})

#######################################################
# 모든 결과 값을 보기
print('-' * 50)
#######################################################
# MongoDB에서 데이터 모두 보기
all_users = list(db.users.find({}))

# 참고) MongoDB에서 특정 조건의 데이터 모두 보기
same_ages = list(db.users.find({'age': 21}))

print('1', all_users[0])  # 0번째 결과값을 보기
print('2', all_users[0]['name'])  # 0번째 결과값의 'name'을 보기

for user in all_users:  # 반복문을 돌며 모든 결과값을 보기
    print(user)

#######################################################
# 특정 결과 값을 뽑아 보기
print('-' * 50)
#######################################################
user = db.users.find_one({'name': 'bobby'})
print('3', user)

# 그 중 특정 키 값을 빼고 보기
user = db.users.find_one({'name': 'bobby'}, {'_id': False})
print('4', user)

#######################################################
# 수정하기
print('-' * 50)
#######################################################
# # 생김새
# db.people.update_many(찾을조건,{ '$set': 어떻게바꿀지 })

# 예시 - 오타가 많으니 이 줄을 복사해서 씁시다!
db.users.update_one({'name': 'bobby'}, {'$set': {'age': 19}})

user = db.users.find_one({'name': 'bobby'})
print('5', user)

#######################################################
# 삭제하기 (거의 안 씀)
print('-' * 50)
#######################################################
db.users.delete_one({'name': 'bobby'})

user = db.users.find_one({'name': 'bobby'})
print(user)

#######################################################
# 정리
print('-' * 50)
#######################################################
# 저장 - 예시
doc = {'name': 'bobby', 'age': 21}
db.users.insert_one(doc)

# 한 개 찾기 - 예시
user = db.users.find_one({'name': 'bobby'})

# 여러개 찾기 - 예시 ( _id 값은 제외하고 출력)
same_ages = list(db.users.find({'age': 21}, {'_id': False}))

# 바꾸기 - 예시
db.users.update_one({'name': 'bobby'}, {'$set': {'age': 19}})

# 지우기 - 예시
db.users.delete_one({'name': 'bobby'})
