#!/bin/env python3
# -*- coding: utf-8 -*- (for python2.x)

import os
import sys
import requests
from requests.adapters import HTTPAdapter
import logging
from pyquery import PyQuery

import sdb


class Crawler:
	proxies = {
		#  'http': 'http://10.0.0.3:10888',
		#  'https': 'https://10.0.0.3:10888',
		#  'http': 'http://10.0.1.1:10888',
		#  'https': 'https://10.0.1.1:10888',
	}
	# url_main = 'http://konachan.wjcodes.com/index.php?tag='
	url_main = 'https://gelbooru.wjcodes.com/index.php?tag='
	url_page = '&p='
	url_rating = 'rating:'
	rating_types = [
		# Level
		{'code': 'safe',		   'remark': 'safe', },
		{'code': 'questionable',   'remark': '15X', },
		{'code': 'explicit',	   'remark': '18X', },
		# Censor
		{'code': 'censored',	   'remark': 'censored', },
		{'code': 'uncensored',	 'remark': 'uncensored', },
		{'code': 'decensored',	 'remark': 'decensored', },
	]
	rating = ''
	download_path = sys.path[0] + '/../download/'
	request_header = {
		'Connection': 'keep-alive',
		'Cookie': 'cf_clearance=CyyN66CAOZDRZCc9y22a9xoYqt3P0a36b7gBlNOXRes-1647657550-0-150; PHPSESSID=dni5mqvfklooh23uct4j3o85e5',
		'Host': 'gelbooru.wjcodes.com',
		'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) Gecko/20100101 Firefox/98.0',
	}

	def __init__(self):
		self.log = logging.getLogger('root.crawler')
		self.session = requests.session()
		# self.session.mount('http://', HTTPAdapter(max_retries=5))
		# self.session.mount('https://', HTTPAdapter(max_retries=5))

	def set_rating(self, types):
		self.rating = ''
		if len(types) > 0:
			self.rating = self.url_rating
		for t in range(len(types)):
			if t > 0:
				self.rating += '%20'
			self.rating += types[t]['code']
			self.log.info('Append rating type: %s(%s)'
						  % (types[t]['code'], types[t]['remark']))
		self.log.info('Set rating: %s' % (self.rating))

	def get_images_in_page(self, page):
		url = self.url_main + self.rating + self.url_page + str(page)
		self.log.debug('  url: %s' % url)
		resp = requests.get(url, headers=self.request_header, proxies=self.proxies)
		if resp.status_code != 200:
			print(' [x] token失效, 返回:', resp)
			print(' [!] 使用浏览器访问:', url, ', 并更新Cookie(cf_clearance和PHPSESSID)')
			return []
		main_list = PyQuery(resp.content)('#main-list')
		addr_list = [i('.am-btn-secondary').attr(
			'onclick')[14:-1].replace('\'', '').split(',') + [url]
					 for i in main_list('li').items()]
		self.log.debug('  got %d images' % len(addr_list))
		return addr_list

	def get_images(self):
		image_list = []
		page = 1
		while True:
			li = self.get_images_in_page(page)
			if len(li) == 0:
				print(';', flush=True)
				break
			image_list += li
			page += 1
			if len(li) == 100:
				print('=', end='', flush=True)
			else:
				print('-', end='', flush=True)
		self.log.debug('  total got %d images' % len(image_list))
		return image_list

	def download(self, image, path=None):
		if path is None:
			path = self.download_path
		if not os.path.exists(path):
			os.makedirs(path)
		# self.request_header['Referer'] = image[2]
		# response = self.session.get(url=image[0], headers=self.request_header)
		response = requests.get(url=image[0], proxies=self.proxies)
		name = path + '/' + image[1] + os.path.splitext(image[0])[1]
		with open(name, 'wb') as fp:
			fp.write(response.content)
		self.log.debug('  download file: %s => %s' % (image[0], name))


def test_get_page_1(p):
	p.set_rating([p.rating_types[2]])
	li = p.get_images_in_page(1)
	for i in range(len(li)):
		print('  %3d: %s' % (i, li[i]))


def test_count_images(p):
	p.set_rating([Crawler.rating_types[2], Crawler.rating_types[4]])
	li = p.get_images()
	print('got %d images' % len(li))


def test_download_first_image(p):
	p.set_rating([])
	li = p.get_images_in_page(1)
	p.download(li[0])


def main():
	sdb.logger_init()

	crawl = Crawler()
	menu = [
		{'name': '----', 'param': 0, 'func': 0, },
		{'name': 'get page 1', 'param': crawl, 'func': test_get_page_1, },
		{'name': 'count images', 'param': crawl, 'func': test_count_images, },
		{'name': 'download first image', 'param': crawl,
		 'func': test_download_first_image, },
	]
	sdb.menu(menu)


if __name__ == '__main__':
	main()
else:
	pass
