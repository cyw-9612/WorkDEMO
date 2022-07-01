#!/bin/env python3
# -*- coding: utf-8 -*- (for python2.x)

import os
import logging
import glob
import shutil

import sdb
import crawler


class FileManager:
    dlp = crawler.Crawler.download_path
    rt = crawler.Crawler.rating_types
    file_classify = [
        {'index': 0, 'code': None, 'path': 'selected', },
        {'index': 1, 'code': [rt[2], rt[4]], 'path': 'uncensored', },
        {'index': 2, 'code': [rt[2], rt[3]], 'path': 'censored', },
        {'index': 3, 'code': [rt[0]], 'path': 'safe', },
        {'index': 4, 'code': [], 'path': 'other', },
        {'index': 5, 'code': None, 'path': 'deleted', },
    ]
    image_list = []
    record_file = '/fn.txt'

    def __init__(self):
        self.log = logging.getLogger('root.file_manager')
        for i in range(len(self.file_classify)):
            self.image_list += [[]]
            path = self.dlp + self.file_classify[i]['path']
            if not os.path.exists(path):
                os.makedirs(path)
                continue
            for root, dirs, files in os.walk(path):
                for f in files:
                    name = os.path.splitext(f)[0]
                    if name.isdigit():
                        self.image_list[i] += [name]
            if self.file_classify[i]['path'] == 'deleted':
                # 将不需要的图片移到此目录下。
                # 启动程序时读取记录文件，删除目录内的所有图片，并将编号追加到记录文件
                fo = None
                try:
                    fo = open(path + self.record_file, 'r+')
                except IOError:
                    fo = open(path + self.record_file, 'w+')
                fn = [line.strip() for line in fo.readlines() if len(line.strip()) > 0]
                self.image_list[i] = list(set(self.image_list[i] + fn))  # 去重
                # 写记录
                fo.truncate(0)
                fo.seek(0)
                for n in self.image_list[i]:
                    fo.write(n + '\n')
                # fo.writelines([n + '\n' for n in self.image_list[i]])
                fo.close()
                # 清空图片
                for _, _, files in os.walk(path):
                    for f in files:
                        name = os.path.splitext(f)[0]
                        if name.isdigit() and name in self.image_list[i]:
                            os.remove(path + '/' + f)
        print('     type\t\t| count')
        for n in range(len(self.image_list)):
            print('  %d. %s\t| %d' % (n, self.file_classify[n]['path'], len(self.image_list[n])))

    def check_exist(self, fc, sn):
        for img in range(len(self.image_list)):
            if sn in self.image_list[img]:
                if fc['index'] < img and self.file_classify[img]['path'] != 'deleted':
                    src = self.dlp + self.file_classify[img]['path'] +  '/' + sn + '.*'
                    src = glob.glob(src)[0]
                    dst = self.dlp + self.file_classify[fc['index']]['path']
                    self.log.debug('%s => %s' % (src, dst))
                    shutil.move(src, dst)
                    self.image_list[img].remove(sn)
                    self.image_list[fc['index']] += [sn]
                    print('m[%d=>%d]' % (img, fc['index']), end='', flush=True)
                return True
        return False

    def save(self, fc, image):
        if self.check_exist(fc, image[1]):
            return
        crawl = crawler.Crawler()
        crawl.download(image, self.dlp + fc['path'])
        self.image_list[fc['index']] += [image[1]]
        print('o', end='', flush=True)


def test_check_exist(p):
    print(p.check_exist(p.file_classify[3], '283839'))

def test_deleted_init(p):
    print(p.image_list[5])

def main():
    sdb.logger_init()

    filemgr = FileManager()
    menu = [
        {'name': '----', 'param': 0, 'func': 0, },
        {'name': 'check exist', 'param': filemgr, 'func': test_check_exist, },
        {'name': 'deleted init', 'param': filemgr, 'func': test_deleted_init, },
    ]
    sdb.menu(menu)


if __name__ == '__main__':
    main()
else:
    pass

