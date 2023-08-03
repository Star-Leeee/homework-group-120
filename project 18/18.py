#send a tx on Bitcoin testnet...
import requests

url='https://en.bitcoin.it/wiki/Testnet'
tx={'tx':'111222333'}
post_html=requests.post(url,data=tx)
#输出并解析返回的响应信息
print(post_html.text)
