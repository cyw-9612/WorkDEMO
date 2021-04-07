#ifndef RSA_H
#define RSA_H

#include <QWidget>
#include <QDebug>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include <openssl/bn.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include "openssl/ssl.h"

// define rsa public key
#define BEGIN_RSA_PUBLIC_KEY    "BEGIN RSA PUBLIC KEY"
#define BEGIN_PUBLIC_KEY        "BEGIN PUBLIC KEY"
#define KEY_LENGTH              1024                        // 密钥长度

QT_BEGIN_NAMESPACE
namespace Ui { class rsa; }
QT_END_NAMESPACE

class rsa : public QWidget
{
    Q_OBJECT

public:
    rsa(QWidget *parent = nullptr);
    ~rsa();

    /**
    * @brief createRsaKey 生成秘钥对
    * @param strPubKey 公钥
    * @param strPriKey 私钥
    * @return 状态
    */
    static bool createRsaKey (QString& strPubKey, QString& strPriKey);
    /**
    * @brief rsa_pri_encrypt 私钥加密
    * @param strClearData 明文
    * @param strPriKey 私钥
    * @return 加密后数据(base64格式)
    */
    static QString rsa_pri_encrypt_base64 (const QString& strClearData, const QString& strPriKey);
    /**
    * @brief rsa_pub_decrypt 公钥解密
    * @param strDecrypt 待解密数据(base64格式)
    * @param strPubKey 公钥钥
    * @return 明文
    */
    static QString rsa_pub_decrypt_base64 (const QString& strDecryptData, const QString& strPubKey);
    /**
    * @brief rsa_pub_encrypt 公钥加密
    * @param strClearData 明文
    * @param strPubKey 私钥
    * @return 加密后数据(base64格式)
    */
    static QString rsa_pub_encrypt_base64 (const QString& strClearData, const QString& strPubKey);
    /**
    * @brief rsa_pri_decrypt 私钥解密
    * @param strDecrypt 待解密数据(base64格式)
    * @param strPriKey 私钥
    * @return 明文
    */
    static QString rsa_pri_decrypt_base64 (const QString& strDecryptData, const QString& strPriKey);
    /**< 测试 */
    static void test ();

    /**
     * @brief RSASignature::createRSA 载入密钥
     * @param key 密钥
     * @param publi 公钥1 私钥0
     * @return
     */
    static RSA * createRSA(unsigned char * key,int publi);

    /**
     * @brief RSASignature::public_encrypt 公钥加密
     * @param data 待加密数据
     * @param keystr 公钥
     * @param encrypted 加密后数据
     * @return
     */
    static int public_encrypt(QString &data,QString &keystr,QString &encrypted);

    /**
     * @brief RSASignature::private_decrypt 私钥解密
     * @param data 待解密数据
     * @param keystr 私钥
     * @param decrypted 解密后的数据
     * @return
     */
    static int private_decrypt(QString &data,QString &keystr,QString &decrypted);

    /**
     * @brief RSASignature::private_encrypt 私钥加密
     * @param data 待加密数据
     * @param keystr 私钥
     * @param encrypted 解密后的数据
     * @return
     */
    static int private_encrypt(QString &data,QString &keystr,QString &encrypted);

    /**
     * @brief RSASignature::public_decrypt 公钥解密
     * @param data 待解密数据
     * @param keystr 公钥
     * @param decrypted 解密后的数据
     * @return
     */
    static int public_decrypt(QString &data,QString &keystr,QString &decrypted);

private:
    Ui::rsa *ui;
};
#endif // RSA_H
