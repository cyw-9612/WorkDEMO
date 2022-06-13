#include "myrsa.h"

myRSA::myRSA()
{

}

/**
* @brief createRsaKey 生成秘钥对
* @param strPubKey 公钥
* @param strPriKey 私钥
* @return 成功状态
*/
//bool myRSA::createRsaKey (QString& strPubKey, QString& strPriKey)
//{
//     RSA *pRsa = RSA_generate_key(KEY_LENGTH, RSA_3, NULL, NULL);
//     if ( !pRsa ){
//         return false;
//     }
//     BIO *pPriBio = BIO_new(BIO_s_mem());
//     PEM_write_bio_RSAPrivateKey(pPriBio, pRsa, NULL, NULL, 0, NULL, NULL);
//     BIO *pPubBio = BIO_new(BIO_s_mem());
//     PEM_write_bio_RSAPublicKey(pPubBio, pRsa);
//     // 获取长度
//     size_t nPriKeyLen = BIO_pending(pPriBio);
//     size_t nPubKeyLen = BIO_pending(pPubBio);
//     // 密钥对读取到字符串
//     char* pPriKey = new char[nPriKeyLen];
//     char* pPubKey = new char[nPubKeyLen];
//     BIO_read(pPriBio, pPriKey, nPriKeyLen);
//     BIO_read(pPubBio, pPubKey, nPubKeyLen);
//     // 存储密钥对
//     strPubKey = QByteArray(pPubKey, nPubKeyLen);
//     strPriKey = QByteArray(pPriKey, nPriKeyLen);
//     // 内存释放
//     RSA_free(pRsa);
//     BIO_free_all(pPriBio);
//     BIO_free_all(pPubBio);
//     delete[] pPriKey;
//     delete[] pPubKey;
//     return true;
//}

/**
 * @brief rsa_pri_encrypt 私钥加密
 * @param strClearData 明文
 * @param strPriKey 私钥
 * @return 加密后数据(base64格式)
 */
QString myRSA::rsa_pri_encrypt_base64 (const QString& strClearData, const QString& strPriKey)
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
    delete[] pEncryptBuf;
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
QString myRSA::rsa_pub_decrypt_base64(const QString& strDecryptData, const QString& strPubKey)
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
    delete[] pClearBuf;
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
QString myRSA::rsa_pub_encrypt_base64 (const QString& strClearData, const QString& strPubKey)
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
    delete[] pEncryptBuf;
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
QString myRSA::rsa_pri_decrypt_base64(const QString& strDecryptData, const QString& strPriKey)
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
    delete[] pClearBuf;
    BIO_free_all(pKeyBio);
    RSA_free(pRsa);
    return strClearData;
}


/**
 * @brief RSASignature::createRSA 载入密钥
 * @param key 密钥
 * @param publi 公钥1 私钥0
 * @return
 */

RSA * myRSA::createRSA(unsigned char * key,int publi)
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

int myRSA::public_encrypt(QString &data,QString &keystr,QString &encrypted)
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
        subdata.resize(exppadding);
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
        b +=RSA_private_encrypt(exppadding,smldata,smlencrypted,rsa,RSA_PKCS1_PADDING);
        if(b>0)
        {
            QByteArray subarray=QByteArray::fromRawData((const char *)smlencrypted,rsasize);
            signByteArray.append(subarray);
        }
        free(smldata);
    }
    QString str(signByteArray.toBase64());
    //qDebug()<<str;
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
int myRSA::private_decrypt(QString &data,QString &keystr,QString &decrypted)
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
        subdata.resize(rsasize);
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
int myRSA::private_encrypt(QString &data,QString &keystr,QString &encrypted)
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
    if(data_len>exppadding-11)//实际一次只能加密117位
        exppadding=exppadding-11;
    int b=0;
    int s=data_len/(exppadding);//片数
    if(data_len%(exppadding))
        s++;
    for(int i=0;i<s;i++)
    {
        //分片加密
        QByteArray subdata;
        subdata.resize(exppadding);
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
        b +=RSA_private_encrypt(exppadding,smldata,smlencrypted,rsa,RSA_PKCS1_PADDING);
        if(b>0)
        {
            QByteArray subarray=QByteArray::fromRawData((const char *)smlencrypted,rsasize);
            signByteArray.append(subarray);
        }
        free(smldata);
    }
    QString str(signByteArray.toBase64());
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
int myRSA::public_decrypt(QString &data,QString &keystr,QString &decrypted)
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
        subdata.resize(rsasize);
        for(int j=0;j<rsasize;j++)
        {
            if(i*rsasize+j>=data_len)
            {
                rsasize = j ;
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

/**
* @brief encrypt 加密接口
* @param byteHex 公钥
* @param strPriKey 私钥
* @return 状态
*/
QByteArray myRSA::Pri_encrypt (QByteArray& byteHex, QString& strPriKey)
{
    QString stringFull = byteHex;
    QString strClearData = "";
    myRSA::private_encrypt(stringFull, strPriKey,strClearData);
    QByteArray AES_TEXT = strClearData.toUtf8();
    return AES_TEXT;
}

/**
* @brief encrypt 私钥解密接口
* @param byteHex 公钥
* @param strPriKey 私钥
* @return 状态
*/
QString myRSA::Pri_decrypt (QByteArray& byteHex, QString& strPriKey)
{
    QString stringFull = byteHex;
    QString strClearData = "";
    myRSA::private_decrypt(stringFull, strPriKey,strClearData);
    return strClearData;
}

