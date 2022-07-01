#!/bin/env python3
# -*- coding: utf-8 -*- (for python2.x)

import sys
import logging
import requests

import sdb
import crawler
import file_manager


crawl = None
fm = None
fast_resume = True
page_limit = 1000   # 最大页数
page_batch = 100    # 查找末页时, 每次跳过的页数
find_batch = 70     # 查找新图片时, 每次跳过的页数


# 找到最后一页。限制为page_limit
def find_last_page():
    global crawl
    global fm
    global page_limit
    global page_batch
    print('find last page:', end='', flush=True)
    start = 1
    end = page_batch
    while True:
        li = crawl.get_images_in_page(start)
        if len(li) < 100:
            print('s;')
            return start
        li = crawl.get_images_in_page(end)
        if len(li) == 100:
            print('>', end='', flush=True)
            start = end + 1
            end += page_batch
            if end >= page_limit:
                break   # 限制在1000页内
            continue
        elif len(li) > 0:
            print('e;')
            return end
        else:
            break
    print('[%d~%d]' % (start, end), end='', flush=True)
    while True:
        last = (end + start) // 2
        print(',%d' % (last), end='', flush=True)
        li = crawl.get_images_in_page(last)
        if len(li) == 100:
            if last == (end - 1):
                print('-;')
                return last
            start = last
            continue
        elif len(li) == 0:
            if last == (start + 1):
                print('+;')
                return start
            end = last
            continue
        else:
            print('!;')
            return last


# 查找存在新图片的页面
def find_page_with_new_image(classify, start, end=None):
    global crawl
    global fm
    global find_batch
    if end is None:
        end = start
    print('find new image', end='', flush=True)
    last = 0
    while True:
        if last == 0:
            last = start
        elif ((end + start) // 2 - last) > find_batch:
            last = start + find_batch
        else:
            last = (end + start) // 2
        li = crawl.get_images_in_page(last)
        print(',%d' % (last), end='', flush=True)
        for i in range(len(li)):
            if fm.check_exist(classify, li[i][1]):
                continue
            print('!;')
            return last
        else:
            if end == last:
                print('$;')
                return 0
            elif last == (end - 1):
                start = end
            else:
                start = last


# 下载处理
def download(p=None):
    global crawl
    global fm
    global fast_resume
    global page_limit
    fc = fm.file_classify
    count_page = True
    if p is not None:
        if 'fc' in p:
            fc = p['fc']
        if 'count_page' in p:
            count_page = p['count_page']
            if not count_page:
                print('Do not find last page.')
    for c in fc:
        if c['code'] is None:
            continue
        crawl.set_rating(c['code'])
        page = 1
        end_page = None
        if fast_resume:
            if count_page:
                end_page = find_last_page()
            page = find_page_with_new_image(c, 1, end_page)
            if page == 0:
                continue
        recheck = 0
        while True:
            print('%4d: ' % (page), end='', flush=True)
            li = crawl.get_images_in_page(page)
            print('<%3d>' % (len(li)), end='', flush=True)
            if len(li) == 0:
                break
            download_count = 0
            for i in li:
                if fm.check_exist(c, i[1]):
                    print('.', end='', flush=True)
                    continue
                while 1:
                    try:
                        crawl.download(i, crawl.download_path + c['path'])
                        break
                    except requests.exceptions.ConnectionError:
                        continue
                    except requests.exceptions.ChunkedEncodingError:
                        continue
                fm.image_list[c['index']] += [i[1]]
                print('o', end='', flush=True)
                download_count += 1
                recheck = 0
            page += 1
            if page > page_limit:
                break
            print()
            if fast_resume and download_count == 0:
                if recheck < 3:
                    recheck += 1
                    continue
                recheck = 0
                page = find_page_with_new_image(c, page + 1, end_page)
                if page == 0:
                    break


# 设置下载模式: 快速查询/遍历查询
def set_resume_mode(p):
    global fast_resume
    print('current: %r' % (fast_resume))
    fast_resume = not fast_resume
    print('set to: %r' % (fast_resume))


def entry(p):
    global crawl
    global fm
    global fast_resume
    menu = [
        {'name': 'all', 'func': download, },
        {'name': 'all-faster', 'param': { 'count_page': False },
            'func': download, },
    ]
    for fc in fm.file_classify:
        if fc['code'] is not None:
            menu += [{'name': fc['path'], 'param': { 'fc': [fc], },
                'func': download, }]
    menu += [{'name': '---- option', }]
    menu += [{'name': 'set fast resume mode', 'func': set_resume_mode, }]
    sdb.menu(menu)


def test_last_page(p):
    global crawl
    global fm
    crawl.set_rating(fm.file_classify[3]['code'])
    page = find_last_page()
    print('last page: %d' % (page))


def test_new_image_page(p):
    global crawl
    global fm
    c = fm.file_classify[3]
    crawl.set_rating(c['code'])
    page = find_last_page()
    print('last page: %d' % (page))
    page = find_page_with_new_image(c, 1, page)
    print('start page: %d' % (page))


def crawler_test(p):
    crawler.main()


def file_manager_test(p):
    file_manager.main()


def main():
    global crawl
    global fm
    sdb.logger_init(sdb.log_file_default)
    print('Python version:', sys.version)

    crawl = crawler.Crawler()
    fm = file_manager.FileManager()
    entry(0)
    # menu = [
    #     {'name': 'download', 'param': 0, 'func': entry, },
    #     {'name': '---- test', 'param': 0, 'func': 0, },
    #     {'name': 'tests get last page', 'param': 0, 'func': test_last_page, },
    #     {'name': 'tests get new image page', 'param': 0,
    #      'func': test_new_image_page, },
    #     {'name': 'tests of crawler', 'param': 0, 'func': crawler_test, },
    #     {'name': 'tests of file manager', 'param': 0,
    #      'func': file_manager_test, },
    # ]
    # sdb.menu(menu)


if __name__ == '__main__':
    main()
else:
    pass
