# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# https://doc.scrapy.org/en/latest/topics/items.html

import scrapy
from scrapy.loader import ItemLoader
from scrapy.loader.processors import MapCompose, TakeFirst, Join


class JavaItemLoader(ItemLoader):
    default_output_processor = TakeFirst()

class JavaItem(scrapy.Item):
    url = scrapy.Field()
    title = scrapy.Field()
    salary_min = scrapy.Field()
    salary_max = scrapy.Field()
    job_city = scrapy.Field()
    experience_year = scrapy.Field()
    education_need = scrapy.Field()
    publish_date = scrapy.Field()
    job_advantage_tags = scrapy.Field()
    position_info = scrapy.Field()
    job_classification = scrapy.Field()
    crawl_time = scrapy.Field()
