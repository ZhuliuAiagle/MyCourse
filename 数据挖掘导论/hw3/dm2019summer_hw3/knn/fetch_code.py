from urllib.request import *

# get new test data


for i in range(10):
    urlretrieve("http://jwbinfosys.zju.edu.cn/CheckCode.aspx","Test/" + str(i) + ".gif")
