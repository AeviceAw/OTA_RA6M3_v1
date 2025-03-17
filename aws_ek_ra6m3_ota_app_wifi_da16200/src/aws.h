/*
 * aws.h
 *
 *  Created on: 21 May 2024
 *      Author: awlir
 */

#ifndef AWS_H_
#define AWS_H_
#include "string.h"

//#define DN 0 // M4005070
//#define DN 1 // M6030070
//#define DN 8 // MF18F070
//#define DN 22 // MD240070
#define DN ota // OTA

//#define ENDPOINT_YY "LAPTOP-QAL5N7L2"
#ifdef  ENDPOINT_YY
#define DNAME "YY_DNAME"
#define ROOT_CA "-----BEGIN CERTIFICATE-----\n"\
"MIID+DCCAuCgAwIBAgITb8eXr+M9fFCe69NexU+Llcrg7DANBgkqhkiG9w0BAQsF\n"\
"ADCBijELMAkGA1UEBhMCU0cxEzARBgNVBAgMClNvbWUtU3RhdGUxETAPBgNVBAcM\n"\
"CFRhaSBTZW5nMRYwFAYDVQQKDA1BZXZpY2UgSGVhbHRoMQwwCgYDVQQLDANSTkQx\n"\
"CzAJBgNVBAMMAllZMSAwHgYJKoZIhvcNAQkBFhF5aXlhbmdAYWV2aWNlLmNvbTAg\n"\
"Fw0yNDA1MjgwMTIyNTRaGA8yMDUxMTAxMzAxMjI1NFowgYoxCzAJBgNVBAYTAlNH\n"\
"MRMwEQYDVQQIDApTb21lLVN0YXRlMREwDwYDVQQHDAhUYWkgU2VuZzEWMBQGA1UE\n"\
"CgwNQWV2aWNlIEhlYWx0aDEMMAoGA1UECwwDUk5EMQswCQYDVQQDDAJZWTEgMB4G\n"\
"CSqGSIb3DQEJARYReWl5YW5nQGFldmljZS5jb20wggEiMA0GCSqGSIb3DQEBAQUA\n"\
"A4IBDwAwggEKAoIBAQDiwGi8LOd8KQEYtYQ95SrLALQSGuG8KQwfJAQgwSvCT1cz\n"\
"7Hhxr1asIk9d2G5CLT+tkKmPSfObUwlbxp43JsDjLdBahA23NXFP5/EuNkjOHGb1\n"\
"DJ3kfgNK0szOYj+TTFiY+4CRNU7gphdskm81RwTiQk+TDEGVU9oSgW8If+XIlNND\n"\
"Fh6hM+7ZhSut8KGfjWUr+ohckHyOnbHn//ivk3wZRMSjOt2RtvrC/XwwWKtJkVQa\n"\
"kxEavS5Rm9rE2wN4hqMDy/Z2YvnNN7c/wYvXGx86TGxvzeBjnbEzkVzavCxU5319\n"\
"l1sw4PU/jI4liUbVvn+kc6tRBeg4EZhGkZh6Gi19AgMBAAGjUzBRMB0GA1UdDgQW\n"\
"BBSYKnqcPMJHBsOyqXCgvgByYFbb4jAfBgNVHSMEGDAWgBSYKnqcPMJHBsOyqXCg\n"\
"vgByYFbb4jAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQBJtbXR\n"\
"MyiDcqal6LEsRNY8rWOuTF3gxBLO/F75FZ0pOgYk7F8kUQe2I3q/UEnfSGTFNche\n"\
"Na4MjHlKtffASf/GztGhYbw6LSTH26bdE/sddnfR2IJ4GqSIAVHgKB60KKg86iTM\n"\
"udjrzAnPQZhrGAH5PMPWX+cxf4PPmybh89+zYHsQ6hyAP7TiKdSwWMOs94cG2iMh\n"\
"CDVIAQhe08mR/+NA9lFqrrdAGxj5Dg4ngBeZHTmS4LrtcNQZ3Uvs8WoXznQEttOk\n"\
"SWZNIwApryEtIPqt9qIMmm5sM1QY4CgXzttDdGzg3ccQ0lOSsPZIWtKW+5EL3V/5\n"\
"zTp40rc0A1RjC8YG\n"\
"-----END CERTIFICATE-----\n"

#elif (DN==ota)
    #define DNAME "LR_DNAME"
    #define DISABLE_SNI (false)
    #define AWS_ENDPOINT "a3diwcjwt78ey7-ats.iot.ap-southeast-1.amazonaws.com"
    #define MQTT_HOST_PORT (8883)
    #define MQTT_SEND_TIMEOUT (5000)
    #define MQTT_RECEIVE_TIMEOUT (5000)
    #define MQTT_TOPIC "ra6m3_ota_demo_thing/Ethernet/Message"
    #define MQTT_CLIENT_IDENTIFIER "ra6m3_ota_demo_thing"
    #define MQTT_KEEP_ALIVE_SECONDS (60)
    #define MQTT_PAYLOAD ("{\"DEVICE_NAME\":\"EK-RA6M3\",\"CONNECTION\":\"OK\",\"ERROR\":\"0\"}")

    /* see pdf pg 523 https://docs.aws.amazon.com/freertos/latest/userguide/dev-guide-ota-workflow.html */
    #define NUMBER_OF_SUBSCRIPTIONS 2
    #define SUB_STREAM  "$aws/things/" MQTT_CLIENT_IDENTIFIER "/streams"
    #define SUB_JOBS    "$aws/things/" MQTT_CLIENT_IDENTIFIER "/jobs"

    #define mqttexampleOUTGOING_PUBLISH_RECORD_LEN (128*1)
    #define mqttexampleINCOMING_PUBLISH_RECORD_LEN (128*1)

    #define ROOT_CA  "-----BEGIN CERTIFICATE-----\n"\
    "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n"\
    "ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n"\
    "b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n"\
    "MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n"\
    "b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n"\
    "ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n"\
    "9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n"\
    "IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n"\
    "VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n"\
    "93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n"\
    "jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n"\
    "AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n"\
    "A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n"\
    "U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n"\
    "N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n"\
    "o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n"\
    "5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n"\
    "rqXRfboQnoZsG4q5WTP468SQvvG5\n"\
    "-----END CERTIFICATE-----"

    #define DEVICE_CERT "-----BEGIN CERTIFICATE-----\n"\
    "MIIDWjCCAkKgAwIBAgIVAITYFXftMQ/4ddyQ8mkRregWq5lWMA0GCSqGSIb3DQEB\n"\
    "CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"\
    "IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDEwMTcwMTIz\n"\
    "NTRaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"\
    "dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC4FD3+EMj+uUDigpY+\n"\
    "NXrz0BSvOHuKwOXTaWwHh6o+bQnomBP37mtJGyAC8AxBh8Djojc17+STxv76Qr0b\n"\
    "uyTnCk24HlIoKegUr4Rax/Oot4bVVZscERGIabz27OOdEuGzOZoXk8HHIW4MBxG/\n"\
    "LcEf7IiIwgNBtEPWdPenSJiZL46EPEcyUGedwuh8jeFoq3NSTQ0aCMHqaVlf5bGc\n"\
    "A9/A+scphKdt1+1YbS0zBvpizvrq2ddPRXiqba2Cf0qdA0ajly85aGDOh4nJ+KV3\n"\
    "82cqR8Ja5EnBufV2JNsfvF/Qtz+pASY2R4TUXZpLoMFmGnkk4IGhnqN0HV9nDjZU\n"\
    "TjORAgMBAAGjYDBeMB8GA1UdIwQYMBaAFDa83J5y4gpffHkuzUvHmLvsmFhRMB0G\n"\
    "A1UdDgQWBBRZVC//bGdF45/Mleh+2+56CaSqnDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"\
    "DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAik5Lgg9D9bKvpv7hP07rG7c2\n"\
    "+goaOgiggNB0lsZlBYA0Q6zZ92yQ+e2xcrMNhZEwfu28H2pLmTAJKMZM0dq/1ztP\n"\
    "rqeCFSxdZFsD+Y9brf4W6aA81ZdeMSFF03LMKP73nOGZ1tJxcQcPOV6zoJd6Hr9p\n"\
    "YdjtzO2ZpSdns+Ym88bWG3axlTW7f/ePbhi+wjJB+xbWdm+/l+xunn0QBy8rRo5P\n"\
    "nkCOjkg/AzV1ZFBi/gQMzBN+iu3qR3NSHK6F6BykETEQWnScytooTozHGzStzx7Z\n"\
    "IDwMGmSWbu1ks78iIMZbtMEs3vu2kjUq3cbzonUCWaLujZQArKI6tMsEnmZXwA==\n"\
    "-----END CERTIFICATE-----\n"

    #define DEVICE_KEY "-----BEGIN RSA PRIVATE KEY-----\n"\
    "MIIEpQIBAAKCAQEAuBQ9/hDI/rlA4oKWPjV689AUrzh7isDl02lsB4eqPm0J6JgT\n"\
    "9+5rSRsgAvAMQYfA46I3Ne/kk8b++kK9G7sk5wpNuB5SKCnoFK+EWsfzqLeG1VWb\n"\
    "HBERiGm89uzjnRLhszmaF5PBxyFuDAcRvy3BH+yIiMIDQbRD1nT3p0iYmS+OhDxH\n"\
    "MlBnncLofI3haKtzUk0NGgjB6mlZX+WxnAPfwPrHKYSnbdftWG0tMwb6Ys766tnX\n"\
    "T0V4qm2tgn9KnQNGo5cvOWhgzoeJyfild/NnKkfCWuRJwbn1diTbH7xf0Lc/qQEm\n"\
    "NkeE1F2aS6DBZhp5JOCBoZ6jdB1fZw42VE4zkQIDAQABAoIBAQCj47nR80XQrEVd\n"\
    "piB4IItoedGAs8winF3C3XCGP75OOCyDPqtAqbAxdYoJs4ll6qTMxsdbKUucWqb3\n"\
    "T+Yow3U2ktDhZXWGNtFfYWl4nd+NTmEonUAtO/EdP+8y9XQti/sQ4+8SoMX9MgFV\n"\
    "ihG94dG5R5HEGOBnZkSjsp4v/Q2IC/HNWqCIe+S6sVA48QOhaJ1vWhnHwjeo0py8\n"\
    "HdKgPPn4fJFiv2hZnsB3o+Vy8x6GWGVyzQxHWXLjF7vydeKqlN9IcvHqJ37ztqq/\n"\
    "SSPVpL3ZeBrLzMGnmO1vnnJu3OiCyZzgLKfcecDOVVdukhVaG32pXBiLui4XvsqD\n"\
    "98Eep6FBAoGBANli3hr4fdbtwXZwQ6YYCZzKv8QEnozVP1LQr3f0SavA9YRkOpyI\n"\
    "s9gR6R76AU0lYyQ4R3XOQi51AVMBgTtyMLL1bGKfdd4yXUh03UGb8x1LIygH9wPT\n"\
    "qWXJERZH+4V5Ozx9Vvy+Xp6+j/ns0Q+aOZAoNaKMjp+T628pmoqk4XhpAoGBANjG\n"\
    "z2hQq5GwjctQEvuuyNjVQTHAim9wcDQz4aMtFXtfRdtifqxdO3Mx1QLnaOeAoc8T\n"\
    "Zj5MOrS8kd2VzXSEtYht/eWk57v6iQDZ5FYnc/bjOBTpmcUyqLYkcO9a8rNoysod\n"\
    "Cydn3wNCzderYLl6VuxYD/P2cJP8Adw4r5ogBzzpAoGBAKPc8+FOuMTfmXpm64dz\n"\
    "DSPkbSMb/Evanw4VAn+gZDwUir2OM+Q046QW9CwGdh6p9Q6jYbJ6lMaiKipgswKV\n"\
    "dAIiH95DpzyjqE9q0DGZlWvabJjLUDbRQRrZsoiiIAWfubhCHuULtxVgPfU9vuVM\n"\
    "8uEofi2WWBgQD2NO8BBccx0pAoGBAM8MdSXphzXMMGK0xKznsKaKaY6fnJn8Bu+V\n"\
    "bjpw4UfJI+v7E4qGMjZArnfrl2MmlWY33Tw7iwb95USDxI06BDme9RbWfxAop5rM\n"\
    "gy9W14hEVliDFoAndsUh4Ux8zbtWp/qL6w9kQrnWoM0Rv3zowQVUx0f5ZmWyGgTB\n"\
    "j9fiJ4ehAoGAXYmqTC1rMUkLSextJbND76XIGwMDpH3eM5lIAuLT2bo5neX44FY7\n"\
    "1YFFvCq6AhpCgyXDvvq839DFsovb3qzOlJfEuxDjggWBg8EzVWiqG/yXUIC+CZrU\n"\
    "c/hy3y2hcfWzx2/47RS9Szm13vQcfx5o2yryJWoGntbJPpoyavjqqb4=\n"\
    "-----END RSA PRIVATE KEY-----\n"

    #define SIGN_KEY "-----BEGIN PUBLIC KEY-----\n"\
    "MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE0zUXdx/TMY2oFCAV0J/ghaZXQfM5\n"\
    "yrWEaMAqpy9N1riSgvlELORawNaeFrFr9A7Tgx0CelSL/v5BtLND0wkUVg==\n"\
    "-----END PUBLIC KEY-----\n"

#else
    #define AWS_ENDPOINT "a13pmlz832iuc9-ats.iot.ap-southeast-1.amazonaws.com"
    #if (DN==0)
        #define DNAME "M4050070"

        #define DEVICE_CERT "-----BEGIN CERTIFICATE-----\n"\
        "MIIDWjCCAkKgAwIBAgIVAL8SSTSsmmJESO8ovbKNxK2JY0yVMA0GCSqGSIb3DQEB\n"\
        "CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"\
        "IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMTA4MTUwNjE1\n"\
        "MzJaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"\
        "dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCcPubkfrhTqKuvjEmk\n"\
        "6dEtqnvVmKjj2bSx/53n3QP+hi2mqXXUz6CDwJ5zv3kv1hDFoHg+JvvOnsqxLE5C\n"\
        "ujcNt3dsLAbIFtsdMzNBqZSnNw9WAuHxf8xoNmdegPsq0dvoT57JuHYevhAXpsjA\n"\
        "cZP2/PLD0UcN19WK1ZLxjQ02tOjhpsgaguAiddIh2fRzWYAJhDD0Ht9NfEfreWPj\n"\
        "ixjFeADN8XY22PhMppHrgZvoCSoAo2aauyLaY6lq4KS3k/vh6TYUUq5bRfvYIHEl\n"\
        "C5AEbzAVsxQtlQwc01D4z8iVSUdgpyVzwdn/O+YjfcUNY1Rduo/v+LdUV6ec71hd\n"\
        "wDJrAgMBAAGjYDBeMB8GA1UdIwQYMBaAFIPhBiwIbnhcg36nudMlccJD4okhMB0G\n"\
        "A1UdDgQWBBRpNnc8zm4GcPSR6OB6t6VHxEYzuDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"\
        "DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAGqiCK2OXVQNRoFcL3Y3vUCT9\n"\
        "ttD+KBMIaF73IYqXhjWW55ivduaOhwu1BfR8u/gNIPo1kzIZgDiT+iJQDeMIcnh9\n"\
        "o3v+Onm9rE9AJ8kBcwykSAPZR22Ul8nGEIdJZNGpWSjoFF+qhZySnLhnpKyTTgUI\n"\
        "Hd7SZ6VBV41LbVRUq5PSsIHWxBpHJAvwH4gyD/0mp1xz75eQOKPm9UBRsxkvFves\n"\
        "C6VDHlF9QBPngUV0RMJ0+z7EqzdSjCnp2vVGNaEBPfZGaFcWGY5SHsPcZ5MFoY1I\n"\
        "6gpy1Gxg9t4EtHs9z0DsUfY8iF+la8sOC0yQAwC+nHEBSsAB/NP/9DYovgKttQ==\n"\
        "-----END CERTIFICATE-----"

        #define PRIVATE_KEY "-----BEGIN RSA PRIVATE KEY-----\n"\
        "MIIEpAIBAAKCAQEAnD7m5H64U6irr4xJpOnRLap71Zio49m0sf+d590D/oYtpql1\n"\
        "1M+gg8Cec795L9YQxaB4Pib7zp7KsSxOQro3Dbd3bCwGyBbbHTMzQamUpzcPVgLh\n"\
        "8X/MaDZnXoD7KtHb6E+eybh2Hr4QF6bIwHGT9vzyw9FHDdfVitWS8Y0NNrTo4abI\n"\
        "GoLgInXSIdn0c1mACYQw9B7fTXxH63lj44sYxXgAzfF2Ntj4TKaR64Gb6AkqAKNm\n"\
        "mrsi2mOpauCkt5P74ek2FFKuW0X72CBxJQuQBG8wFbMULZUMHNNQ+M/IlUlHYKcl\n"\
        "c8HZ/zvmI33FDWNUXbqP7/i3VFennO9YXcAyawIDAQABAoIBAHF3OKQM5n1Si0OT\n"\
        "5FNPvamlGgL2tweorRFqqVidZ24qMPLMM5bVJSMo+ajjDvXPRiTOsSOpmcnmBRp/\n"\
        "K2JH6v0aFC7JFi+ucXcWHI39wcdhM9DpjP2dKmsvvwehMUVaW5GsaIrjW1xd+D/E\n"\
        "zhr7IFIIRaAw1aHW8Ou+x9SE82ia+9xPsFJ0rq1n0QDb4tSOLIb0XaJokyODRRzc\n"\
        "UGAdhBzmgQHdxxkl7ufpn5E+PwK0VD8UDQ4OSUZFaE25/hIQ1u9rvBmdBGL1rbNz\n"\
        "JZYus9yzEm/iVJZVYuMIUmyB2v95tFm1zs1aTelBgFOLJ6A78RNNPmXJLv7kVUEo\n"\
        "Jf12mLkCgYEAyNxYLYBR54XEcid9nbwIAAidhcTf9ujYozcb70Z0yNqniriXj/6+\n"\
        "My84uud0MR5IhTrgW51W1PFxU7xhO7znX1Uh6n2ld5UhLHiOSTuMSAhtXfB976Mr\n"\
        "+wQzJSffhHVHh5yuv+zNZEdlEoStJI9tzzv3npDFL/V0BC6As1mZWc8CgYEAxyMz\n"\
        "Q8TurirDo7z+MVoBI18/jtP2xfIm0PeYGzJ4wwC67Kpi/Wne0OVKpJVl1xfmvjJS\n"\
        "z4JG2PPeGfVhOtwd8iIDflvJjjxYf4WxOtgdxB9zBn5SX+/PGqhfE//G4LhrtxZI\n"\
        "jQNbyFMs6Hlq/wveX29YfATzRIrSmN1PwgblsKUCgYB6+t5KTazYMoDK2/7/WqcT\n"\
        "TjMUOixegsZReGZ0JBFRAP9f4hF6KqDNufXkCd6MTFxepmPD9Bu8GXAnssldyBVp\n"\
        "uzge2Hs9MmOzVMTZdco4OGCuCZcx6Bf/gtzFvRoPgbcgAwDDpftDkKsDoZRZNDAp\n"\
        "mC/OOBljecdk4AwQw6Uu5QKBgQCmNKf1+SkpLsIpKHWUZUWr6zSWFi3SWuCw6uYG\n"\
        "VSLbxwE8xJ1oWvPAsI+TPKpKU0WgG0porZhKPYhzbA1h3hQUYvU9goiAcbd/f+tD\n"\
        "bNuzNHiBw+8m6XWXD06D16h7KRGmNzAImfQCCAHkamq2QoawarOUl2EfJSrOsXB/\n"\
        "RdhxjQKBgQCJEKRZHl1ifp103hfhktci4H4AsvvHf0YCj/oGyz8LhKnV9EprPzLf\n"\
        "RXQ/qdDQH1FDuj4C41fS9M2y54+KwDw4Bk+Dl4yfDTFemOUffXQidK927UfRbKvx\n"\
        "yDgzAToTCXAMNf2CR7YUTElhg2VlThBShjvyAgBioaToOGv02gQOKA==\n"\
        "-----END RSA PRIVATE KEY-----"
    #elif (DN==1)
        #define DNAME "M6030070"

        #define DEVICE_CERT "-----BEGIN CERTIFICATE-----\n"\
        "MIIDWTCCAkGgAwIBAgIUXKmlYF3ZbdakLKcYCvxVlIYgFTMwDQYJKoZIhvcNAQEL\n"\
        "BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"\
        "SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIxMDgxNzAzMjMx\n"\
        "NVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"\
        "ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKiKu7yVlD3AtxemJeZn\n"\
        "/j9Hvf7lmztn/WMd4qe08KMtxi8nWTF16jYBf5m2LmGVeom3rJ6FYK/BM9wfJmQU\n"\
        "2Qz+aO/hokW2VfAvafXYKHV59k2A5hqCC9vHgJ+bSn25k8k70YYa3MRrpktQ9VQa\n"\
        "NeHdaOVY3+Dlny2Zi/jU6JpPWxmYg30pAE6viP9PrEyTaUgglvgYxZSEkGLj6iLg\n"\
        "3sIja1luxNUvyYuYVfNLcv7jfPjLIzjkmKmVldTtHHDY52Rq/YUnVNQRxTShsdjb\n"\
        "NgHp1GW3YHC3A2lE8GBmaud5LX7wluMGP9gO47qbkMeHB3X9/nmbpK2LXKO/Lwys\n"\
        "y1ECAwEAAaNgMF4wHwYDVR0jBBgwFoAU3ohUhiM1dDrmTemG4IGHUF1+xycwHQYD\n"\
        "VR0OBBYEFLlxp237bpOVv0KBInQdQD1SzopdMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"\
        "AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBGoZ1czwyxEyvstgP1WYzz6L8p\n"\
        "MntVvfWFjXU0I4naqGm1H/BRfaA6hGn6U2APDvxbd7Dob57HulzjlR0OMa7BT/Tb\n"\
        "eHJmcmaQ0Qpbq218/RjV/9i6N92brXcn/Ws+0v5vhRIf60BG9e7CjbEp5NERV3vP\n"\
        "i29rh4CcskzqrHBfmOjsnfH2tOWEjL9EG2emdxIFFCCRNZP/9iXORdnVnt26IUdC\n"\
        "zUXBrw3gpsC1lVoBe1B054Qudx/Ts/UBS6S3YwLk/SB1vURElohYzBIv5yG93xTF\n"\
        "FOgeK+FMBtrmxlX6X9h6540PTtqS+c1AwhaAthmPc1ueEVwsCM1Yse+gxheE\n"\
        "-----END CERTIFICATE-----"


        #define PRIVATE_KEY  "-----BEGIN RSA PRIVATE KEY-----\n"\
        "MIIEowIBAAKCAQEAqIq7vJWUPcC3F6Yl5mf+P0e9/uWbO2f9Yx3ip7Twoy3GLydZ\n"\
        "MXXqNgF/mbYuYZV6ibesnoVgr8Ez3B8mZBTZDP5o7+GiRbZV8C9p9dgodXn2TYDm\n"\
        "GoIL28eAn5tKfbmTyTvRhhrcxGumS1D1VBo14d1o5Vjf4OWfLZmL+NTomk9bGZiD\n"\
        "fSkATq+I/0+sTJNpSCCW+BjFlISQYuPqIuDewiNrWW7E1S/Ji5hV80ty/uN8+Msj\n"\
        "OOSYqZWV1O0ccNjnZGr9hSdU1BHFNKGx2Ns2AenUZbdgcLcDaUTwYGZq53ktfvCW\n"\
        "4wY/2A7jupuQx4cHdf3+eZukrYtco78vDKzLUQIDAQABAoIBAQCIZz9pcOV41O9Z\n"\
        "hG5H+wIZ1MY/9TOc8JxyC03UzOu0JOewYfgdv4C+FJhgnsEK2R9DNhPOd8O4enSu\n"\
        "QcegV/8SNRRYkBP0t4z1xmnLPhYAfaSJNTkFWxqd1OiUQ+oVtouU4YTBNBxYG4eK\n"\
        "jQr/9faFxduPQk5uhWg+T3ao/CcEOsTlYwt+JjLb5ISmSKuPoGjfmdLjM5lB+Ta+\n"\
        "hAmUsSNN3mheYS8h2yVZCZAF1tp+51rVpWhHt2eL4vYe41MoIPqCT02y0KhDMf5R\n"\
        "68q+4UcqFY2Grv5GscyZFHC90JKffvC6nAFWujxtLxB50owRG+WLMvIZ0sKtKZwS\n"\
        "19c3oh3BAoGBANbiO4IRIYHeZgbBY2EHyy1EkKiyPc38CtjHIP6/zXexklKygZt/\n"\
        "4kgvLasaaX8YDBFxrMPDEXsvGP6t8w0ZmrEFnAGy210gh6UUXXy1ip5H0wQXdPS5\n"\
        "Hug/JNvMVF933/BJV5JXNqofJMRkNlI7+atFoblBOt2v0zF57bRhczrLAoGBAMjK\n"\
        "g+wt7rMsSzSAbSxWt76Z1BuyBBFcnUaXYoEBpMc23KtRPPJVo6dCFppmthZktXir\n"\
        "pRAWTJfdIRHGtY09MsU4YWbYQEDQhswp8qyhXWIAOZr2Ka9WrT/ywArqF64SJixU\n"\
        "Cj+v5WyaWvWEGRKCZrcz3y3p+LH8gHHrF8hIQULTAoGAA6VjDRntT57ObF7rqMHR\n"\
        "GLuJGNOtaMlWBcebL7dhbR5+NRkCSfkphcAHLlgMYjOuQL2FCO1OZqCt8+RPs3Ez\n"\
        "xr0poH3AaxwaMRrfWzZZW8YsNm2PdneNprMrlO8qn1QRYKpd6gXAz7InlY19vnd2\n"\
        "DQnpbJNGxGwvUS+YcO8WXXECgYAoTQ2BgA6jA8bseMJw3c7vgKykCmzqly4yFvx8\n"\
        "bDoIiihsclfLRAke7xLeVrNBd2E9ISZ2PiONb6LNkJasDJZ+IzzZiZoL6E7F971v\n"\
        "jMvTebwWMEZZnQoVBcIp7qZmF9p1l0CfW8yuqaSMFsF5h5baOp0W+BkkkEuvkcIq\n"\
        "PuZqZQKBgCO9EeCRUnK0Uoy9nKkCyFdCVSS8vilmzWLq34zkHciOA+vp+oq2kmRS\n"\
        "Wiyn4LmOf7o4PdrckUtjLs7j3uTItZTSUoRbCb4zLcp6qu7/mVEQdV/V5pw7o8EI\n"\
        "byd5FJfYMJ5itsVc7g0mgxxYlhmg30BDelDgKjN3ZnvrrBesJOLM\n"\
        "-----END RSA PRIVATE KEY-----"

    #elif (DN==8)
        #define DNAME "MF18F070"

        #define DEVICE_CERT "-----BEGIN CERTIFICATE-----\n"\
        "MIIDWTCCAkGgAwIBAgIUS1UKBNSbulrLEz69Ve5J+RwGNvUwDQYJKoZIhvcNAQEL\n"\
        "BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"\
        "SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIzMDgwODAzMTc0\n"\
        "NFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"\
        "ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMT7A0jULHPFb8XZrFYJ\n"\
        "jhh8gym+/cWi6cFI7tirfkxEvCBc77kiLII2mxREt5mBGqtb0EzYW0RrYua9iT0B\n"\
        "OifwJOD1Q3fKRbC99Zpueqh0NmJ5bCW7nAlNiKeUSLnmQI8ZpqIXAOmjQrKCmpu1\n"\
        "R6xVvWDQTp8mfYDtdrYS1dJKgdx/TdBRdD1FtsJ8XA37wDAINx7jLflSh6XBi7+s\n"\
        "9gb5ytvF3xNckzdPshlfXAdNcybvkTuwuHF9A3gsj/qS3qB5M4RS1lDy8Rb7of2o\n"\
        "Io1qYYVfnMskoBgux9kTbHGOAzeujFMt3FviiDnEMWuaN1gdsH6oD1YN6w8/FjW/\n"\
        "++cCAwEAAaNgMF4wHwYDVR0jBBgwFoAU5uYySSJp6+4EzmM3dUXl/HHMlnUwHQYD\n"\
        "VR0OBBYEFLnKpx4lbShEB9L/dNw9EzOPLOzBMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"\
        "AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBCP9QOu5Tn9E/VLaOxLxyVXGXM\n"\
        "Q/CekgfVXS7S2zQWp4V11+Z14cG7jVQTSxj+yL23HtnT4fV32jdMvrlMJvCrYEcH\n"\
        "rJVO84mZYNa+X5nB0ViPSRUMsMVjm69a2KP44glUDC8hsk/FkVLEXt9gzqiJuL04\n"\
        "s80UpWbObhYFgJIim3hs+wRImzeHUAcyWljoD5oBqG50aTqZZjM0zlJ9LMME1odM\n"\
        "dh6tKG5PipltH2uNaNMkZdqAixYyC4FuGBHeXaCjpSvJ8gYRQZBjXSRAwRe+Xk8+\n"\
        "4OC+BoKHEb4u+CmJxgCYYDtjlEoLp3bJS8BSjFi6oFHlm9qT1hnoslouzCyH\n"\
        "-----END CERTIFICATE-----"

        #define PRIVATE_KEY "-----BEGIN RSA PRIVATE KEY-----\n"\
        "MIIEpQIBAAKCAQEAxPsDSNQsc8VvxdmsVgmOGHyDKb79xaLpwUju2Kt+TES8IFzv\n"\
        "uSIsgjabFES3mYEaq1vQTNhbRGti5r2JPQE6J/Ak4PVDd8pFsL31mm56qHQ2Ynls\n"\
        "JbucCU2Ip5RIueZAjxmmohcA6aNCsoKam7VHrFW9YNBOnyZ9gO12thLV0kqB3H9N\n"\
        "0FF0PUW2wnxcDfvAMAg3HuMt+VKHpcGLv6z2BvnK28XfE1yTN0+yGV9cB01zJu+R\n"\
        "O7C4cX0DeCyP+pLeoHkzhFLWUPLxFvuh/agijWphhV+cyySgGC7H2RNscY4DN66M\n"\
        "Uy3cW+KIOcQxa5o3WB2wfqgPVg3rDz8WNb/75wIDAQABAoIBAQCA67x5lQxG9Uiz\n"\
        "WET3xqculYeO8gbLrFWLgH16Z7twlWf/UnQ/WgOBcdXLzTHREPNwhdXuBYiuXg5L\n"\
        "8G7682upqf2pEj0HEObNx3YQjhBPYkgZ6ViD1adJW2p+K4nxyaYlQmeSHLYnNAg6\n"\
        "J0qjcGEphLA+xfc/KITptgGFJwy05ZFwZqjwL6qMiwzN7EiIQKwCHukdi14YyHMY\n"\
        "ugFu0aEzdfAnG65mu9g1IQObyZ7iQeU4VsCRf7dgwhMVVMmerSrBW+jcogbxONpV\n"\
        "jq29DRvhXgwroR1MZEGMnzJztgXaIDqP+cMwhOghamzKia/a4aUsS0Fuk9Kf16fP\n"\
        "KlW9oks5AoGBAPgphuZLrL/DgoPIs51zMF7S5AdSvjdVDtY4N8dNSEN6vCgYZv3g\n"\
        "4bUgxj2U0EW31U/C0w2Gu0okUk+B1XPbUniL4nBAJufF/bksQ7zUJMV7CUNKe1L4\n"\
        "aajvkVy8GMHm/YhZB0IjGct3QnQCZSpyHDKMUUP0ajR2ULznE6umQXWNAoGBAMsz\n"\
        "qkC+JQy79CrSuImsmk6cz+T5we1M4PKW05a8g4efe1tQ3nfCDYirWB9Wvp1wuegQ\n"\
        "eioMiVO6QOBVnp9vScva+oEcJ5DF0aHByAsxrDqsZI8A9hqzOeN2v3qydBIe9Gjk\n"\
        "UksuznosM0Zf24lR2b1nNieeXYcat2k29tkBPRhDAoGAPBT5xyoNMgRoxfVCn10z\n"\
        "GdIfSTKcQRND21OTFZq9YzEE+wQlc1QN8Wd095Xe8vI2H/CAXqb/E01F7F/3yOwR\n"\
        "xIPBmAHAKZ9C9aEk6UPHf1Rxgqw2vXjNRAx0eipEKDX/rhI38qhC54MBQMsPqT4l\n"\
        "W4ujZTt4tyjqFUIw/f7rnEUCgYEAnRpDviyEXpUOUPF8JU0iPDc28HQ8/+fZXxVH\n"\
        "X5o+IuQlrlMWvQZ2Cj1XM1uQog7eXNIocUwh/G7a69amfeO/7XFPDfUjjeWzjtlQ\n"\
        "fX8ZBn/0ey+mFryMvBU6M3W3Ea+HDXXBPklg78oulnFUPZXfv3UFxJnG+8anGRya\n"\
        "uuofPxsCgYEAoNPlO2zHy8BNUYE8PoTqPiP+UJlYi7u/IAXDwuxVcvP9RTPIYcdK\n"\
        "XADHwUqB5P5Fr5TY/mKtlLJdPw6yNwKv+LEvy4LxiGrwrSG8EOnJXOjR/fzJ8jq+\n"\
        "mOpFlsyQNNfx7Z/Mrg87xQeSASK9S9wBdYfwFTBai6IjhJQPhZv9ZQg=\n"\
        "-----END RSA PRIVATE KEY-----"

#elif (DN==22)
        #define DNAME "MD240070"

        #define DEVICE_CERT "-----BEGIN CERTIFICATE-----\n"\
        "MIIDWTCCAkGgAwIBAgIUHu5ZbcxsY6CzI+Z94HK8cMfRch4wDQYJKoZIhvcNAQEL\n"\
        "BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"\
        "SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIzMDgyOTA5MzU1\n"\
        "OVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"\
        "ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKBr4TNnwbJD7qhO764h\n"\
        "N/fEFS+8HMWD7xg/rgVvYe6XndEpVM3QPwbK62wyRLrz4Uj9AHnhubKaK9XeP270\n"\
        "URLcnpKwd+2gJvNYPhkUplEZfoOYRDhVtaK8+eIOdnG/zd1VlR5wk53SnNR8DbIB\n"\
        "SKAzF4g4j1w+r+CKhj916Hqr5MNEMrXJy4GoXRfk7byFZxfMZ++ifmLUhpQ/nvmB\n"\
        "/TU0b6aM8EAGb8uE2LpgYpctJF2IXrvPQQ6fS281UlfVlZelYm0VbNNLTFh0BjLH\n"\
        "BbeiFYR7+K4cwi9fz8w+qmDSjhURb6KKKQHDz4F+BSkxgReELt25wp6fyAAuyMa9\n"\
        "WwkCAwEAAaNgMF4wHwYDVR0jBBgwFoAU1i1+yORyEePKaVKPBlVAAJYKbpwwHQYD\n"\
        "VR0OBBYEFFoV6zx4p3PUY2zuG6EytIqdPNyKMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"\
        "AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQB/jTw1/cD/4EoDEKMw5eCukoBi\n"\
        "nAVbcu42reuAxjHeboXDlH90Xy24PqaUPAIjpBPf32NkjtkFv7a0en2wX4iH3QKX\n"\
        "dzO6r7m8aG3ZZnxnTPc5CRzuoGWIcNqHdfZmCZiFrdZzMDfVTFzozbrz486e43Dd\n"\
        "CiBibDByD73gaW/iqyGEPvkwFSopRCfq0hWximeIT2qxKGWpZJ5bz/C8dkGuoRbP\n"\
        "fptxN7TiiFE5ApKm7cZrcRyZ8PfRWEBTlOEWDFNsLXxp/lArNQid6tHtz1zcpx15\n"\
        "phNaJC8jGI4X9Gyt+RI80QA+xPjTDEyNjvo3BmB4dMf1O0lkRocl5A0BRRIH\n"\
        "-----END CERTIFICATE-----"

        #define PRIVATE_KEY "-----BEGIN RSA PRIVATE KEY-----\n"\
        "MIIEowIBAAKCAQEAoGvhM2fBskPuqE7vriE398QVL7wcxYPvGD+uBW9h7ped0SlU\n"\
        "zdA/BsrrbDJEuvPhSP0AeeG5spor1d4/bvRREtyekrB37aAm81g+GRSmURl+g5hE\n"\
        "OFW1orz54g52cb/N3VWVHnCTndKc1HwNsgFIoDMXiDiPXD6v4IqGP3Xoeqvkw0Qy\n"\
        "tcnLgahdF+TtvIVnF8xn76J+YtSGlD+e+YH9NTRvpozwQAZvy4TYumBily0kXYhe\n"\
        "u89BDp9LbzVSV9WVl6VibRVs00tMWHQGMscFt6IVhHv4rhzCL1/PzD6qYNKOFRFv\n"\
        "ooopAcPPgX4FKTGBF4Qu3bnCnp/IAC7Ixr1bCQIDAQABAoIBAQCHrnbMyab0JazH\n"\
        "QexKdOIndvsMYqhbDlm9Qq7m/evwpt/eNnz5jnagQiSYYHPtQ7R5zTMH4zjyrJ8C\n"\
        "xnUUXSPtNhgQrZzhqKEJFMuA3wxEb7Gnj7OzihqUSA15ew4G61ktKON02Xag96+Z\n"\
        "sErNEXThuhTfLCdkt8h9u+wL5Kf6h+08jRgeUx2Wx52ELkRB+95SW+ImaG98lY3K\n"\
        "QBN6mMr7K+bk3rDGGi9TBL0HG7Ct+0jFbNLTQ5R2+SAFwIordnEl4njsA+MLDYQC\n"\
        "dr6eiHkcAXIFaei5oRADTU4RR1TSTuZkLXb+4C4s98ZPsGSNWnnbeim9Olm8waA8\n"\
        "/y5wmWBhAoGBANT3VSdOUNXOCkTGsLnCAIu7s/2I+AS/j1pL4y2w365GUNXT8yG2\n"\
        "zOJ7J9pvMMdz2HKBM4JLssrT0D3y2U+EVsu5HbFILeJCH6qojXkppLdFAISnfHxp\n"\
        "MkYDGF3HMOdQSKcduc8DzddpZsnqGQ9gb2QuCAl2e5KLhnrK5eTQXAf9AoGBAMDW\n"\
        "bOJ4Ex+lkmsP5Bbvy1xL1mgUy/pP3Zt+2/owiKQY6atQUQyv81q6K98VP5+swdpJ\n"\
        "hcAuktyB0Iamq8M4lK+1e64ojJ4p02Co282tvUQSkwSGAWM4jtkAeyDtMD4ly0Sy\n"\
        "b5Ub/dAD1asCAM204o52D5RZp7I3kadK4hrp4i79AoGAWMmhI9j/1JOpLlFT/q4G\n"\
        "QjuIZ+yB0qD2qCO5XCRArUy9iNhrpiOrOcU0sVnoEU+X7u00x1699NfoGtuzg/Zd\n"\
        "pWPRIEdCcfE1eJX7HYeiF5JaC/Qz7z6AajYCI57XIakUETBQdiFU4TkdVJMXzZRb\n"\
        "/H1AdomCLW+kKm+K9yFzLRUCgYAJ8gUjmPXDiBKJGwS0eiGO3Qpty8TJYB0Zy+0k\n"\
        "diRV8kL65uWHNfvjDf79PmdNoOdOjsQrKTvBy6ApkacCJoTkmwOJ8+VsstOIBK1x\n"\
        "aih8vkh+zzp7F4umfXPcOt9mFSTqohlHI25vsALxEuVftpiIqGYdTJJtipgkfKQ0\n"\
        "pyNXMQKBgCziHn152J8LjxnbvWnvMU7VOelZIoGCrNskdgeTPj+Rmxr2DvkOIKan\n"\
        "hQr+ZmY15R7Pf+nVfvdq6ELpD95lr0jS/hbllg2T0FHCiGz595qIZEL0iSH5DNJf\n"\
        "BqFS6PwQ94E61uKvrRJsDLSDovbbgBxhVqW6MxOfxZLRwdpUPPsx\n"\
        "-----END RSA PRIVATE KEY-----"

    #endif



    #define ROOT_CA "-----BEGIN CERTIFICATE-----\n"\
    "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n"\
    "ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n"\
    "b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n"\
    "MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n"\
    "b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n"\
    "ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n"\
    "9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n"\
    "IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n"\
    "VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n"\
    "93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n"\
    "jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n"\
    "AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n"\
    "A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n"\
    "U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n"\
    "N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n"\
    "o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n"\
    "5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n"\
    "rqXRfboQnoZsG4q5WTP468SQvvG5\n"\
    "-----END CERTIFICATE-----"
#endif

#define ROOT_CA_LEN strlen(ROOT_CA)
#define DEVICE_CERT_LEN strlen(DEVICE_CERT)
#define PRIVATE_KEY_LEN strlen(PRIVATE_KEY)
#define IMU

#endif // AWS_H_
