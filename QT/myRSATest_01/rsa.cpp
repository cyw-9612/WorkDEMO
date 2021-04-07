#include "rsa.h"
#include "ui_rsa.h"

rsa::rsa(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::rsa)
{
    ui->setupUi(this);
    test();
}

rsa::~rsa()
{
    delete ui;
}

/**
* @brief createRsaKey 生成秘钥对
* @param strPubKey 公钥
* @param strPriKey 私钥
* @return 成功状态
*/
bool rsa::createRsaKey (QString& strPubKey, QString& strPriKey)
{
     RSA *pRsa = RSA_generate_key(KEY_LENGTH, RSA_3, NULL, NULL);
     if ( !pRsa ){
         return false;
     }
     BIO *pPriBio = BIO_new(BIO_s_mem());
     PEM_write_bio_RSAPrivateKey(pPriBio, pRsa, NULL, NULL, 0, NULL, NULL);
     BIO *pPubBio = BIO_new(BIO_s_mem());
     PEM_write_bio_RSAPublicKey(pPubBio, pRsa);
     // 获取长度
     size_t nPriKeyLen = BIO_pending(pPriBio);
     size_t nPubKeyLen = BIO_pending(pPubBio);
     // 密钥对读取到字符串
     char* pPriKey = new char[nPriKeyLen];
     char* pPubKey = new char[nPubKeyLen];
     BIO_read(pPriBio, pPriKey, nPriKeyLen);
     BIO_read(pPubBio, pPubKey, nPubKeyLen);
     // 存储密钥对
     strPubKey = QByteArray(pPubKey, nPubKeyLen);
     strPriKey = QByteArray(pPriKey, nPriKeyLen);
     // 内存释放
     RSA_free(pRsa);
     BIO_free_all(pPriBio);
     BIO_free_all(pPubBio);
     delete pPriKey;
     delete pPubKey;
     return true;
}

/**
 * @brief rsa_pri_encrypt 私钥加密
 * @param strClearData 明文
 * @param strPriKey 私钥
 * @return 加密后数据(base64格式)
 */
QString rsa::rsa_pri_encrypt_base64 (const QString& strClearData, const QString& strPriKey)
{
    QByteArray priKeyArry = strPriKey.toUtf8();
    uchar* pPriKey = (uchar*)priKeyArry.data();
    BIO* pKeyBio = BIO_new_mem_buf(pPriKey, strPriKey.length());
    if (pKeyBio == NULL){
        return "";
    }
    RSA* pRsa = RSA_new();
    pRsa = PEM_read_bio_RSAPrivateKey(pKeyBio, &pRsa, NULL, NULL);
    if ( pRsa == NULL ){
         BIO_free_all(pKeyBio);
         return "";
    }
    int nLen = RSA_size(pRsa);
    char* pEncryptBuf = new char[nLen];
    memset(pEncryptBuf, 0, nLen);
    QByteArray clearDataArry = strClearData.toUtf8();
    int nClearDataLen = clearDataArry.length();
    uchar* pClearData = (uchar*)clearDataArry.data();
    int nSize = RSA_private_encrypt(nClearDataLen,
                                    pClearData,
                                    (uchar*)pEncryptBuf,
                                    pRsa,
                                    RSA_PKCS1_PADDING);
    QString strEncryptData = "";
    if ( nSize >= 0 ){
         QByteArray arry(pEncryptBuf, nSize);
         strEncryptData = arry.toBase64();
    }
    // 释放内存
    delete pEncryptBuf;
    BIO_free_all(pKeyBio);
    RSA_free(pRsa);
    return strEncryptData;
}

/**
 * @brief rsa_pub_decrypt 公钥解密
 * @param strDecrypt 待解密数据(base64格式)
 * @param strPubKey 公钥
 * @return 明文
 */
QString rsa::rsa_pub_decrypt_base64(const QString& strDecryptData, const QString& strPubKey)
{
    QByteArray pubKeyArry = strPubKey.toUtf8();
    uchar* pPubKey = (uchar*)pubKeyArry.data();
    BIO* pKeyBio = BIO_new_mem_buf(pPubKey, strPubKey.length());
    if (pKeyBio == NULL){
        return "";
    }

    RSA* pRsa = RSA_new();
    if ( strPubKey.contains(BEGIN_RSA_PUBLIC_KEY) ){
        pRsa = PEM_read_bio_RSAPublicKey(pKeyBio, &pRsa, NULL, NULL);
    }else{
        pRsa = PEM_read_bio_RSA_PUBKEY(pKeyBio, &pRsa, NULL, NULL);
    }

    if ( pRsa == NULL ){
        BIO_free_all(pKeyBio);
        return "";
    }
    int nLen = RSA_size(pRsa);
    char* pClearBuf = new char[nLen];
    memset(pClearBuf, 0, nLen);
    //解密
    QByteArray decryptDataArry = strDecryptData.toUtf8();
    decryptDataArry = QByteArray::fromBase64(decryptDataArry);
    int nDecryptDataLen = decryptDataArry.length();
    uchar* pDecryptData = (uchar*)decryptDataArry.data();
    int nSize = RSA_public_decrypt(nDecryptDataLen,
                                   pDecryptData,
                                   (uchar*)pClearBuf,
                                   pRsa,
                                   RSA_PKCS1_PADDING);
    QString strClearData = "";
    if ( nSize >= 0 ){
        strClearData = QByteArray(pClearBuf, nSize);
    }

    // 释放内存
    delete pClearBuf;
    BIO_free_all(pKeyBio);
    RSA_free(pRsa);
    return strClearData;
}

/**
 * @brief rsa_pub_encrypt 公钥加密
 * @param strClearData 明文
 * @param strPubKey 私钥
 * @return 加密后数据(base64格式)
 */
QString rsa::rsa_pub_encrypt_base64 (const QString& strClearData, const QString& strPubKey)
{
    QByteArray pubKeyArry = strPubKey.toUtf8();
    uchar* pPubKey = (uchar*)pubKeyArry.data();
    BIO* pKeyBio = BIO_new_mem_buf(pPubKey, pubKeyArry.length());
    if (pKeyBio == NULL){
        return "";
    }
    RSA* pRsa = RSA_new();
    if ( strPubKey.contains(BEGIN_RSA_PUBLIC_KEY) ){
        pRsa = PEM_read_bio_RSAPublicKey(pKeyBio, &pRsa, NULL, NULL);
    }else{
        pRsa = PEM_read_bio_RSA_PUBKEY(pKeyBio, &pRsa, NULL, NULL);
    }
    if ( pRsa == NULL ){
        BIO_free_all(pKeyBio);
        return "";
    }

    int nLen = RSA_size(pRsa);
    char* pEncryptBuf = new char[nLen];
    memset(pEncryptBuf, 0, nLen);

    QByteArray clearDataArry = strClearData.toUtf8();
    int nClearDataLen = clearDataArry.length();
    uchar* pClearData = (uchar*)clearDataArry.data();
    int nSize = RSA_public_encrypt(nClearDataLen,
                                   pClearData,
                                   (uchar*)pEncryptBuf,
                                   pRsa,
                                   RSA_PKCS1_PADDING);
    QString strEncryptData = "";
    if ( nSize >= 0 ){
        QByteArray arry(pEncryptBuf, nSize);
        strEncryptData = arry.toBase64();
    }
    // 释放内存
    delete pEncryptBuf;
    BIO_free_all(pKeyBio);
    RSA_free(pRsa);
    return strEncryptData;
}

/**
 * @brief rsa_pri_decrypt 私钥解密
 * @param strDecrypt 待解密数据(base64格式)
 * @param strPriKey 私钥
 * @return 明文
 */
QString rsa::rsa_pri_decrypt_base64(const QString& strDecryptData, const QString& strPriKey)
{
    QByteArray priKeyArry = strPriKey.toUtf8();
    uchar* pPriKey = (uchar*)priKeyArry.data();
    BIO* pKeyBio = BIO_new_mem_buf(pPriKey, priKeyArry.length());
    if (pKeyBio == NULL){
        return "";
    }
    RSA* pRsa = RSA_new();
    pRsa = PEM_read_bio_RSAPrivateKey(pKeyBio, &pRsa, NULL, NULL);
    if ( pRsa == NULL ){
        BIO_free_all(pKeyBio);
        return "";
    }
    int nLen = RSA_size(pRsa);
    char* pClearBuf = new char[nLen];
    memset(pClearBuf, 0, nLen);

    //解密
    QByteArray decryptDataArry = strDecryptData.toUtf8();
    decryptDataArry = QByteArray::fromBase64(decryptDataArry);
    int nDecryptDataLen = decryptDataArry.length();
    uchar* pDecryptData = (uchar*)decryptDataArry.data();
    int nSize = RSA_private_decrypt(nDecryptDataLen,
                                    pDecryptData,
                                    (uchar*)pClearBuf,
                                    pRsa,
                                    RSA_PKCS1_PADDING);
    QString strClearData = "";
    if ( nSize >= 0 ){
        strClearData = QByteArray(pClearBuf, nSize);
    }
    // 释放内存
    delete pClearBuf;
    BIO_free_all(pKeyBio);
    RSA_free(pRsa);
    return strClearData;
}

void rsa::test()
{
    /**< rsa private/public key 若从文件中拷贝出来，需要注意保存元先文件格式，即换行符需要带上，包括最后一行的换行符 */
//    QString strPriKey = "-----BEGIN RSA PRIVATE KEY-----\n"
//                        "MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAIKQFcsUJQR2zPS9\n"
//                        "Qu9KA4ikEp8XdhzE0W6QhlKIuqKDcF7wAOo1UgiHKV05Yyw7+/JH8w9SQWenePoc\n"
//                        "Cu18ZIPHt040taEGGXj88c9cgOqSxPLlOk/OOsecePbJ/mjB244aw6hTSEJSMbln\n"
//                        "XCFMFb20QNM1sXWY8buMA7CgVCh/AgMBAAECgYBQbdo/A+XZ3iVcDssRBZMvtYMT\n"
//                        "JizIhCZP6hAU/82ZF0c99LNblAU446qDlzqUoU8PbWs54P4RkIVh7ax9p8dyymyx\n"
//                        "1oYNVjIezoQwcjMLow7vNjRmPhpdV1AIBohCsL3gnuAUV5kQgqUvlWAtmw6RqJ6A\n"
//                        "S+y6Tpzzmzn+DzvcwQJBAPml/Q9ELg72XXzBqi1DZhs0MbF1CjDQxWGQc/j+gLgZ\n"
//                        "YKfyjNFejlRI7mPApUI9t9JPvAEiJt2a3JsqIIE/fSkCQQCF4nfBYebaObnRynEV\n"
//                        "jLfKaehobdBKPcb4CixidgTyZQGqTx8PfIkG4KMTIbfGmjtA+uaXoCKaN/Acd0tT\n"
//                        "fwVnAkEAryCoE0uK2+gr0NayoKoFONQ2f/xQt8ncjQKeuG8gsQTsbySRcFZalfnL\n"
//                        "caK0y3EsQJa6ljj09sQqOZcl0kVNcQJAS5Y4Fi23r6y3hAHPJmNzwAta8tpMph2K\n"
//                        "n67esBgBYvgzoHtbBdtBYWihqMN4hd+9ZEpCtYfrATZjkS6okFM+MQJBAJ+sA2R0\n"
//                        "wcBdD0DHLA0xYEgpTeJsnYamh66Uc/bi1Zy4RhQU+pV0PuqcHzFkjTe5fcXsBNrs\n"
//                        "lOX5XjXvRYMEe8E=\n"
//                        "-----END RSA PRIVATE KEY-----";
//    QString strPubKey = "-----BEGIN PUBLIC KEY-----\n"
//                        "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCCkBXLFCUEdsz0vULvSgOIpBKf\n"
//                        "F3YcxNFukIZSiLqig3Be8ADqNVIIhyldOWMsO/vyR/MPUkFnp3j6HArtfGSDx7dO\n"
//                        "NLWhBhl4/PHPXIDqksTy5TpPzjrHnHj2yf5owduOGsOoU0hCUjG5Z1whTBW9tEDT\n"
//                        "NbF1mPG7jAOwoFQofwIDAQAB\n"
//                        "-----END PUBLIC KEY-----";
    QString strPriKey = "-----BEGIN RSA PRIVATE KEY-----\n"
            "MIICeAIBADANBgkqhkiG9w0BAQEFAASCAmIwggJeAgEAAoGBAKD2StGJ/WQq/gbx\n"
            "5qF6BgLDa1klY8ISMj4IsXxjCNc0aP9Ly9Z6FTXNnF3CkTizqsdKWMt9z9HXHRJ8\n"
            "gCD+KdJT6IJRShI3yQQg+9dqt/V3d1qjqMNZupCIA7XSYAlUZyhkdRxNos6dgFOg\n"
            "3xxOhIfiO4CSe5X4h6+7+nKoo4dxAgMBAAECgYEAiI0pH+Z+rvHCG8YwMCFC5ClA\n"
            "8pnDraqeMP5TBAFISLnPsXVt8gAT6lcKJoifhoj1gqhR+CA9o/loUnyIfnTgqI/G\n"
            "NDG30uzdGqGGi5ldDH/2FC88YfjkmDfW02Vp0ca6zwBvBTWVYG+ADTjJnmNPHauO\n"
            "zvMvUo7m1W0Sz/GMJ3kCQQDsqdYXRAqx5CyqbmyL7hLGKscmklExdrK/Ad/z7bp5\n"
            "6Eke9PKcW8E3k4gme4Y6GyquDqeCFQXY4WCh1xbOPkI3AkEArh0NDNEQz8u0GQer\n"
            "pYPcdnEk7c/fQADR6a9asFAu7sVi7SP5O21/6rKU5uKZnvQaET5ORpp06Pzvq4c0\n"
            "+KDPlwJBANBrsbPDpHeNrxhZLxb8j2QeAE0jxoV9N2WnzQcRcYwy2VJ9/btdwwei\n"
            "0eCyID/D9Q5csYCJ6yBZO7TLq7XudnkCQA6OYTPsGpK0vgbFrGT3BCxmwyaalVh9\n"
            "eQ8nWNaarYHKwL2An2qHegU+DtLKqPsv/ynn4pVmflWoJ8VYVWZf01sCQQCAnIRF\n"
            "ahfmHFs26UQ77xdHzu4YQiBs/Ol5UDcmV0u+JC5O4qJHRSnS4V0aA3alnP3aHWFx\n"
            "EdTrg04qX6etugPt\n"
            "-----END RSA PRIVATE KEY-----";

    QString strPubKey = "-----BEGIN PUBLIC KEY-----\n"
                        "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCg9krRif1kKv4G8eahegYCw2tZ\n"
                        "JWPCEjI+CLF8YwjXNGj/S8vWehU1zZxdwpE4s6rHSljLfc/R1x0SfIAg/inSU+iC\n"
                        "UUoSN8kEIPvXarf1d3dao6jDWbqQiAO10mAJVGcoZHUcTaLOnYBToN8cToSH4juA\n"
                        "knuV+Ievu/pyqKOHcQIDAQAB\n"
                        "-----END PUBLIC KEY-----";
    /**
     *  用代码生成的key与openssl命令生成的key区别：
     *  1、代码生成key，标题为 -----BEGIN RSA PUBLIC KEY-----,openssl命令生成key, 标题为 -----BEGIN PUBLIC KEY-----
     *  2、获取RSA函数不同，代码生成key，用PEM_read_bio_RSAPublicKey，openssl命令生成key，用PEM_read_bio_RSA_PUBKEY
    */
//    createRsaKey(strPubKey, strPriKey);
    qDebug() << "strPubKey:" << strPubKey << endl;
    qDebug() << "strPriKey:" << strPriKey << endl;
//    qDebug() << "private key encrypt, public key decrypt";
    QString strClear = "<N>{\"cmdID\":1,\"cmdType\":10,\"paraPack\":{\"bios\":\"73C0W33\",\"cpuid\":\"BFEBFBFF000906ED\",\"ip\":\"192.168.0.99\",\"productKey\":\"SVManagement\",\"pwd\":\"admin\",\"user\":\"admin\"},\"timestamp\":\"20210317115304\"}</N>";
//    QString strClear = "1234657";
//    QString strEncryptData = rsa_pri_encrypt_base64 (strClear, strPriKey);
    QString strEncryptData = "QnYzdXA3VmFTdW5KUnZoMHNTL0tFY0ptUFYvZTJFc0poaFpZMkdsY29tSWYrUVRqU3V2U2U2a3VlZ1FnMlBYMmRkZHhaYWpLQnhOem5taDQ0c0Y1c1UySlZnRVhiaEZ1MnE0ajFXemwzOHZ2WDBad3pDUWRpbCtVVmk0VnJLNllyK0VaTFdJd2RpbHdtbE12WnhWSjB2blBUSll6cFVPU2laT29hYUJiM1Y4PQ==";
//     QString strEncryptData = "";
//    int x = private_encrypt (strClear, strPriKey,strEncryptData);
    qDebug() << " 加密后：" <<strEncryptData;
//    QString strClearData = rsa_pub_decrypt_base64 (strEncryptData, strPubKey);
    QString strClearData = "";
    int x = public_decrypt(strEncryptData, strPubKey,strClearData);
    qDebug() << " 解密后：" << strClearData;
//     qDebug() << "public key encrypt, private key decrypt";
//     strClearData = "";
//    strEncryptData = rsa_pub_encrypt_base64 (strClear, strPubKey);
//      strEncryptData = "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";
//    qDebug() << strEncryptData;
//    x = private_decrypt(strEncryptData, strPriKey, strClearData);
//    qDebug() << strClearData;

}

/**
 * @brief RSASignature::createRSA 载入密钥
 * @param key 密钥
 * @param publi 公钥1 私钥0
 * @return
 */
RSA * rsa::createRSA(unsigned char * key,int publi)
{
    RSA *rsa= NULL;
    BIO *keybio ;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL)
    {
        qDebug()<< "Failed to create key BIO";
        return 0;
    }
    if(publi)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }

    if(rsa == NULL)
    {
        qDebug()<< "Failed to create RSA";
    }
    return rsa;
}

/**
 * @brief RSASignature::public_encrypt 公钥加密
 * @param data 待加密数据
 * @param keystr 公钥
 * @param encrypted 加密后数据
 * @return
 */
int rsa::public_encrypt(QString &data,QString &keystr,QString &encrypted)
{
    QByteArray keydata=keystr.toUtf8();
    unsigned char *key= (uchar*)keydata.data();//密钥
    RSA * rsa = createRSA(key,1);
    if(rsa==NULL)
        return 0;
    free(key);

    int rsasize=RSA_size(rsa);  //128
    int exppadding=rsasize;
    int result=-1;
    //QByteArray decdata=QByteArray::fromStdString(data.toStdString()).toBase64(QByteArray::Base64Encoding);
    QByteArray decdata = data.toUtf8();
    QByteArray signByteArray;
    int data_len=decdata.length();  //实际明文的长度
    if(data_len>exppadding-11)//padding占11位 实际一次只能加密117位
        exppadding=exppadding-11;
    int b=0;
    int s=data_len/(exppadding);//片数
    if(data_len%(exppadding))
        s++;
    for(int i=0;i<s;i++)
    {
        //分片加密
        QByteArray subdata={0};
        for(int j=0;j<exppadding;j++)
        {
            if(i*exppadding+j>data_len)
            {
                exppadding = j-1;
               break;
            }

            subdata[j]=decdata[j+i*exppadding];
        }
        unsigned char *smldata=(unsigned char*)subdata.data();//数据分片
        unsigned char smlencrypted[1024]={0};//片段加密数据
        b +=RSA_private_encrypt(exppadding,smldata,smlencrypted,rsa,RSA_PKCS1_PADDING);
        if(b>0)
        {
            QByteArray subarray=QByteArray::fromRawData((const char *)smlencrypted,rsasize);
            signByteArray.append(subarray);
        }
        free(smldata);
    }
    QString str(signByteArray.toBase64());
    qDebug()<<str;
    encrypted.append(str);
    result=b;
    return result;
}

/**
 * @brief RSASignature::private_decrypt 私钥解密
 * @param data 待解密数据
 * @param keystr 私钥
 * @param decrypted 解密后的数据
 * @return
 */
int rsa::private_decrypt(QString &data,QString &keystr,QString &decrypted)
{
    QByteArray keydata=keystr.toUtf8();
    unsigned char *key= (uchar*)keydata.data();//密钥
    RSA * rsa = createRSA(key,0);
    if(rsa==NULL)
        return 0;
    free(key);
    int rsasize=RSA_size(rsa);
    int result=-1;
    QByteArray encdata=QByteArray::fromBase64(data.toUtf8());
    QByteArray signByteArray;
    int data_len=encdata.length();
    int b=0;
    int s=data_len/(rsasize);//片数
    if(data_len%(rsasize))
        s++;
    for(int i=0;i<s;i++)
    {
        QByteArray subdata={0};
        for(int j=0;j<rsasize;j++)
        {
            if(i*rsasize+j>data_len)
                break;
            subdata[j]=encdata[j+i*rsasize];
        }
        unsigned char *smldata=(unsigned char*)subdata.data();//(unsigned char*)strdup(subdata.constData());//数据分片
        unsigned char smlencrypted[1024]={0};//片段加密数据
        b +=RSA_private_decrypt(rsasize,smldata,smlencrypted,rsa,RSA_PKCS1_PADDING);
        if(b>0)
        {
            QByteArray decdata((char*)smlencrypted);
            signByteArray.append(decdata);
        }
    }
    decrypted.append(QString::fromUtf8(signByteArray));
    result=b;
    return result;
}

/**
 * @brief RSASignature::private_encrypt 私钥加密
 * @param data 待加密数据
 * @param keystr 私钥
 * @param encrypted 解密后的数据
 * @return
 */
int rsa::private_encrypt(QString &data,QString &keystr,QString &encrypted)
{
    QByteArray keydata=keystr.toUtf8();
    unsigned char *key= (uchar*)keydata.data();//密钥
    RSA * rsa = createRSA(key,0);
    if(rsa==NULL)
        return 0;
    free(key);

    int rsasize=RSA_size(rsa);  //128
    int exppadding=rsasize;
    int result=-1;
    //QByteArray decdata=QByteArray::fromStdString(data.toStdString()).toBase64(QByteArray::Base64Encoding);
    QByteArray decdata = data.toUtf8();
    QByteArray signByteArray;
    int data_len=decdata.length();  //实际明文的长度
    if(data_len>exppadding-11)//padding占11位 实际一次只能加密117位
        exppadding=exppadding-11;
    int b=0;
    int s=data_len/(exppadding);//片数
    if(data_len%(exppadding))
        s++;
    for(int i=0;i<s;i++)
    {
        //分片加密
        QByteArray subdata={0};
        for(int j=0;j<exppadding;j++)
        {
            if(i*exppadding+j>=data_len)
            {
                exppadding = j;
                break;
            }
            subdata[j]=decdata[j+i*exppadding];
        }
        unsigned char *smldata=(unsigned char*)subdata.data();//数据分片
        unsigned char smlencrypted[1024]={0};//片段加密数据
        b +=RSA_private_encrypt(exppadding,smldata,(uchar*)smlencrypted,rsa,RSA_PKCS1_PADDING);
                qDebug() << b;
        if(b>0)
        {
            QByteArray subarray=QByteArray::fromRawData((const char *)smlencrypted,rsasize);
            signByteArray.append(subarray);
            qDebug() << "subarray" << subarray.toBase64() << endl;
        }
        free(smldata);
    }
    qDebug() << "signByteArray" << signByteArray.toBase64() << endl;
    QString str(signByteArray.toBase64());
    qDebug()<<"str: " << str << endl;
    encrypted.append(str);
    result=b;
    return result;
}

/**
 * @brief RSASignature::public_decrypt 公钥解密
 * @param data 待解密数据
 * @param keystr 公钥
 * @param decrypted 解密后的数据
 * @return
 */
int rsa::public_decrypt(QString &data,QString &keystr,QString &decrypted)
{
    QByteArray keydata=keystr.toUtf8();
    unsigned char *key= (uchar*)keydata.data();//密钥
    RSA * rsa = createRSA(key,1);
    if(rsa==NULL)
        return 0;
    free(key);
    int rsasize=RSA_size(rsa);
    int result=-1;
    QByteArray encdata=QByteArray::fromBase64(data.toUtf8());
    QByteArray signByteArray;
    int data_len=encdata.length();
    int b=0;
    int s=data_len/(rsasize);//片数
    if(data_len%(rsasize))
        s++;
    for(int i=0;i<s;i++)
    {
        QByteArray subdata={0};
        for(int j=0;j<rsasize;j++)
        {
            if(i*rsasize+j>=data_len)
            {
                rsasize = j;
                break;
            }
            subdata[j]=encdata[j+i*rsasize];
        }
        unsigned char *smldata=(unsigned char*)subdata.data();//(unsigned char*)strdup(subdata.constData());//数据分片
        unsigned char smlencrypted[1024]={0};//片段加密数据
        b +=RSA_public_decrypt(rsasize,smldata,smlencrypted,rsa,RSA_PKCS1_PADDING);
        if(b>0)
        {
            QByteArray decdata((char*)smlencrypted);
            signByteArray.append(decdata);
        }
    }
    decrypted.append(QString::fromUtf8(signByteArray));
    result=b;
    return result;
}
