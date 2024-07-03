import re
import pymysql
import urllib.request,urllib.error
from bs4 import BeautifulSoup

urlFirst = "https://lishi.tianqi.com/"
urlThirdOfDate="/"
urlFinal=".html"
cityList={"长沙":"changsha","武汉":"wuhan","北京":"beijing","杭州":"hangzhou","上海":"shanghai","南京":"nanjing"}
city=input("City:")
year=int(input("year:"))
month=int(input("month:"))
day=int(input("day:"))
findLinkDate=re.compile(r'<div class="th200">(.*?)</div>')
findDataEach=re.compile(r'<div class="th140">(.*?)</div>')
dateMax={1:31,2:28,3:31,4:30,5:31,6:30,7:31,8:31,9:30,10:31,11:30,12:31,20:29}
def getURL(address,year,month,day):
    url=urlFirst
    url+=cityList[address]
    url+=urlThirdOfDate
    url+=str(year)
    if(month<10):
        url+='0'+str(month)
    else:
        url+=str(month)
    url+=urlFinal
    return url
def askURL(url):
    head = {"User-Agent": "Mozilla / 5.0(Windows NT 10.0; Win64; x64) AppleWebKit / 537.36(KHTML, like Gecko) Chrome / 80.0.3987.122  Safari / 537.36"}
    request= urllib.request.Request(url,headers=head)
    html=""
    try:
        response=urllib.request.urlopen(request)
        html=response.read().decode("utf-8")
    except urllib.error.URLError as e:
        if hasattr(e,"code"):
            print(e.code)
        if hasattr(e,"reason"):
            print(e.reason)
    return html
def toint(s):
    if(s[0]=='0'):
        return int(s[1])
    return int(s)
def get_data(url):
    html=askURL(url)
    soup=BeautifulSoup(html,"html.parser")
    for item in soup.find_all('ul',class_="thrui"):
        data=[]
        item=str(item)
        link=re.findall(findLinkDate,item)
        datas=re.findall(findDataEach,item)
        conn = pymysql.connect(
            host='localhost',
            port=3306,
            user='root',
            password='zhoujin@MySQL',
            charset='utf8',
            database="data"
        )
        cursor=conn.cursor()
        sql = "INSERT INTO climate(max_temperature, min_temperature,city,year,month,day,weather,wind_direction,wind_strength) VALUES (%s, %s,\'%s\',%s,%s,%s,\'%s\',\'%s\',\'%s\')"
        dats=[]
        for i in range(len(link)):
            datum = []
            datum.append(link[i])
            for j in range(4):
                datum.append(datas[i*4+j])
            eachDatum=(int(datum[1][:len(datum[1])-1]),int(datum[2][:len(datum[2])-1]),city,year,month,toint(datum[0][8:10]),datum[3],datum[4].split()[0],datum[4].split()[1])
            dats.append(eachDatum)
            request=sql%eachDatum
            cursor.execute(request)
        conn.commit()
        cursor.close()
        conn.close()
if __name__ == '__main__':
    get_data(getURL(city,year,month,day))