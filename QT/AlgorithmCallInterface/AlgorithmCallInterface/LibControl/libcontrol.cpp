#include "libcontrol.h"

QString LibControl::ipServerx=QStringLiteral("192.168.0.166");
int LibControl::portServerx=7050;
QString LibControl::currentDayString = NULL;
QString LibControl::localIP = NULL;
QString LibControl::MAC = NULL;
QString LibControl::CPUID = NULL;
QString LibControl::BIOS = NULL;
int LibControl::serialWidgetType = 0;
QString LibControl::strPriKey = "-----BEGIN RSA PRIVATE KEY-----\n"
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

LibControl::LibControl()
{
}
