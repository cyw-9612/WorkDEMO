#!/bin/sh

curl -v https://gelbooru.wjcodes.com/index.php\?tag\=rating:explicit%20uncensored\&p\=2\
    -H 'Connection: keep-alive'\
    -H 'Cookie: cf_clearance=WhuCbwmar2sj7uHOMs5cMdQelvAiHZL7ztFFOHK9_Cw-1647625429-0-150; PHPSESSID=k2ilf8r9mt6ptnmn4biu7124q1; Hm_lvt_1a24c75afab540e93df54d2fbb24adcd=1647623325; Hm_lpvt_1a24c75afab540e93df54d2fbb24adcd=0'\
    -H 'Host: gelbooru.wjcodes.com'\
    -H 'User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) Gecko/20100101 Firefox/98.0'\
    \
    --output log/test_page.html
