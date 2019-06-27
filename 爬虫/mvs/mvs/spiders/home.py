import scrapy
from pyquery import PyQuery as pq
from scrapy.http import Request
import os.path


class HomeSpider(scrapy.Spider):
    
    baseurl = 'https://www.dytt8.net'

    name = 'home'
    start_urls = ['https://www.dytt8.net']
    
    def parse(self,r):
        page = pq(r.text)
        # 解析 ‘更多’ 按钮
        for item in page('.co_area2 .title_all').find('a').items():
            href = item.attr('href')
            os.path.join(self.baseurl,href)
            yield Request(self.baseurl+href,self.parseMore)

    # 
    def parseMore(self,r):
        print(r.url)

    def parseMorePage(self,r):
        page = pq(r.text)
        page('.co_content8')
        pass

