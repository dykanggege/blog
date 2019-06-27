# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# https://doc.scrapy.org/en/latest/topics/items.html

import scrapy


# 定义结构化数据字段，持久爬到的数据

class MvsItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
    pass


class FilmItem(scrapy.Item):
    cname = scrapy.Field()
    name = scrapy.Field()
    time = scrapy.Field()
    ftype = scrapy.Field()
    link = scrapy.Field()
    douban = scrapy.Field()
    imdb = scrapy.Field()
    long = scrapy.Field()
    detail = scrapy.Field()


