/***********************************************************************************************************************
 * File Name    : usr_network.c
 * Description  : Contains Ethernet and network related data structures and functions used in the application
 **********************************************************************************************************************/
/***********************************************************************************************************************
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
#include "common_data.h"
#include "usr_network.h"
#include "common_utils.h"
#include "usr_hal.h"

extern char g_network_if_info[64];

#if (configENABLED_NETWORKS & AWSIOT_NETWORK_TYPE_ETH)
#if ((ipconfigUSE_DHCP != 0) && (ipconfigUSE_DHCP_HOOK != 0))
extern eDHCPCallbackAnswer_t xApplicationDHCPHook(eDHCPCallbackPhase_t eDHCPPhase, uint32_t ulIPAddress);
#endif

#if( ipconfigDHCP_REGISTER_HOSTNAME == 1 )
/* DHCP has an option for clients to register their hostname.  It doesn't
 have much use, except that a device can be found in a router along with its
 name. If this option is used the callback below must be provided by the
 application writer to return a const string, denoting the device's name. 
*/
extern const char* pcApplicationHostnameHook(void);
#endif /* ipconfigDHCP_REGISTER_HOSTNAME */

#if( ipconfigUSE_DHCP != 0 )
IPV4Parameters_t xNd = {RESET_VALUE, RESET_VALUE, RESET_VALUE, {RESET_VALUE, RESET_VALUE}, RESET_VALUE, RESET_VALUE};

/* DHCP populates these IP address, Sub net mask and Gateway Address. So start with this is zeroed out values
 * The MAC address is Test MAC address.
 */
       uint8_t ucMACAddress[6] =       { 0x00, 0x11, 0x22, 0x33, 0x44, 0x54 };
static uint8_t ucIPAddress[4] =        { RESET_VALUE };
static uint8_t ucNetMask[4] =          { 255, 255, 255,  0 };
static uint8_t ucGatewayAddress[4] =   { RESET_VALUE };
static uint8_t ucDNSServerAddress[4] = { RESET_VALUE };
#else
/* Static IP configuration, when DHCP mode is not used for the Example Project.
 * This needs to be populated by the user according to the Network Settings of your LAN.
 * This sample address taken from the LAN where it is tested. This is different for different LAN.
 * get the Address using the PC IPconfig details.
 */
        uint8_t ucMACAddress[ 6 ]       = {0x00, 0x11, 0x22, 0x33, 0x44, 0x54};
static  uint8_t ucIPAddress[ 4 ]        = {192, 168,   0,  21};
static  uint8_t ucNetMask[ 4 ]          = {255, 255, 255,   0};
static  uint8_t ucGatewayAddress[ 4 ]   = {192, 168,   0,   1};
static  uint8_t ucDNSServerAddress[ 4 ] = {  8,   8,   8,   8};
#endif

extern TaskHandle_t app_thread;

extern char g_network_if_info[64];
extern uint8_t g_ether0_mac_address[6];
extern char g_mqtt_endpoint[128];

static uint32_t ulIPAddress = RESET_VALUE;

static char cBuffer[16] = { RESET_VALUE };

static bool is_dhcp_in_use = false;
static bool is_dhcp_complete = false;
ping_data_t ping_data = { RESET_VALUE, RESET_VALUE, RESET_VALUE };

static fsp_err_t network_ip_preinit(void);
static BaseType_t network_ip_init(void);
static void print_ipconfig(void);
static uint32_t isNetworkUp(void);

static void mac_address_conv_hex_str_to_char(char * input, uint8_t * output);
static uint8_t conv_hex_str_to_char(char input);

/*******************************************************************************************************************//**
 * @brief      This is the User Hook for the DHCP Response. xApplicationDHCPHook() is called by DHCP Client Code when DHCP
 *             handshake messages are exchanged from the Server.
 * @param[in]  Different Phases of DHCP Phases and the Offered IP Address
 * @retval     Returns DHCP Answers.
 **********************************************************************************************************************/
#if ((ipconfigUSE_DHCP != 0) && (ipconfigUSE_DHCP_HOOK != 0))
eDHCPCallbackAnswer_t xApplicationDHCPHook(eDHCPCallbackPhase_t eDHCPPhase, uint32_t lulIPAddress)
{
    eDHCPCallbackAnswer_t eReturn = eDHCPContinue;
    /*
     * This hook is called in a couple of places during the DHCP process, as identified by the eDHCPPhase parameter.
     */
    switch (eDHCPPhase)
    {
        case eDHCPPhasePreDiscover:
            /*
             *  A DHCP discovery is about to be sent out.  eDHCPContinue is returned to allow the discovery to go out.
             *  If eDHCPUseDefaults had been returned instead then the DHCP process would be stopped and the statically
             *  configured IP address would be used.
             *  If eDHCPStopNoChanges had been returned instead then the DHCP process would be stopped and whatever the
             *  current network configuration was would continue to be used.
             */
        break;

        case eDHCPPhasePreRequest:
            /* An offer has been received from the DHCP server, and the offered IP address is passed in the lulIPAddress
             * parameter.
             */
            /*
             * The sub-domains don’t match, so continue with the DHCP process so the offered IP address is used.
             */
            /* Update the Structure, the DHCP state Machine is not updating this */
        	xNd.ulIPAddress = lulIPAddress;
            is_dhcp_in_use = true;
            updateDhcpResponseToUsr ();
        break;

        default:
            /*
             * Cannot be reached, but set eReturn to prevent compiler warnings where compilers are disposed to generating one.
             */
        break;
    }
    return eReturn;
}
#endif

/*******************************************************************************************************************//**
 * @brief      Update the DHCP info to the User data structure.
 * @param[in]  None
 * @retval     None
 **********************************************************************************************************************/
#if( ipconfigUSE_DHCP != 0 )
void updateDhcpResponseToUsr(void)
{
    if (is_dhcp_in_use)
    {
        xNd.ulNetMask = FreeRTOS_GetNetmask();
        xNd.ulGatewayAddress = FreeRTOS_GetGatewayAddress();
        xNd.ulDNSServerAddresses[0] = FreeRTOS_GetDNSServerAddress();

        is_dhcp_complete = true;
    }
}
#endif

/*******************************************************************************************************************//**
 * @brief      DHCP Hook function to populate the user defined Host name for the Kit.
 * @param[in]  None
 * @retval     Hostname
 **********************************************************************************************************************/
#if( ipconfigDHCP_REGISTER_HOSTNAME == 1 )
const char* pcApplicationHostnameHook(void)
{
    return KIT_NAME;
}
#endif

/*******************************************************************************************************************//**
 * @brief      Network event callback. Indicates the Network event. Added here to avoid the build errors
 * @param[in]  None
 * @retval     Hostname
 **********************************************************************************************************************/
#if ( ipconfigUSE_NETWORK_EVENT_HOOK == 1 )
void vApplicationIPNetworkEventHook(eIPCallbackEvent_t eNetworkEvent)
{
    if (eNetworkUp == eNetworkEvent)
    {
        uint32_t lulIPAddress;
        uint32_t lulNetMask;
        uint32_t lulGatewayAddress;
        uint32_t lulDNSServerAddress;
        int8_t lcBuffer[16];

        /* Signal application the network is UP */
        xTaskNotifyFromISR(app_thread, eNetworkUp, eSetBits, NULL);

        /* The network is up and configured.  Print out the configuration
         obtained from the DHCP server. */
        FreeRTOS_GetAddressConfiguration (&lulIPAddress, &lulNetMask, &lulGatewayAddress, &lulDNSServerAddress);

        /* Convert the IP address to a string then print it out. */
        FreeRTOS_inet_ntoa (lulIPAddress, (char*) lcBuffer);

        /* Convert the net mask to a string then print it out. */
        FreeRTOS_inet_ntoa (lulNetMask, (char*) lcBuffer);

        /* Convert the IP address of the gateway to a string then print it out. */
        FreeRTOS_inet_ntoa (lulGatewayAddress, (char*) lcBuffer);

        /* Convert the IP address of the DNS server to a string then print it out. */
        FreeRTOS_inet_ntoa (lulDNSServerAddress, (char*) lcBuffer);
    }
}
#endif

/*******************************************************************************************************************//**
 * @brief      User Hook for the Ping Reply. vApplicationPingReplyHook() is called by the TCP/IP
 *             stack when the stack receives a ping reply.
 * @param[in]  Ping reply status and Identifier
 * @retval     None
 **********************************************************************************************************************/
#if ( ipconfigSUPPORT_OUTGOING_PINGS == 1 )
void vApplicationPingReplyHook( ePingReplyStatus_t eStatus, uint16_t usIdentifier )
{
    (void)  usIdentifier;

    switch( eStatus )
    {
        /* A valid ping reply has been received */
        case eSuccess    :
            ping_data.received++;
            break;
            /* A reply was received but it was not valid. */
        case eInvalidData :
        default:
            ping_data.lost++;
            break;
    }
}
#endif

/*******************************************************************************************************************//**
 * @brief       This functions validates and Iniitalizes the WiFi user credential settings.
 * @param[in]   None
 * @retval      fsp_err_t: FSP_SUCCESS or FSP_ERR_NOT_INITIALIZED
 **********************************************************************************************************************/
fsp_err_t network_ip_preinit(void)
{
    fsp_err_t fsp_status = FSP_SUCCESS;
    uint8_t mac_address[6] = {0x0};

#if (NETWORK_SETUP_USE_FIXED_INFO == 1U)
    memcpy(&g_network_if_info, ETHERNET_MAC_ADDRESS, sizeof(ETHERNET_MAC_ADDRESS));
#endif

    if (0 == strlen (g_network_if_info))
    {
        fsp_status = FSP_ERR_INVALID_ARGUMENT;
    }

    mac_address_conv_hex_str_to_char(&g_network_if_info[0], &mac_address[0]);

    memcpy(&ucMACAddress[0], &mac_address[0], sizeof(mac_address));
    memcpy(&g_ether0_mac_address[0], &mac_address[0], sizeof(mac_address));

    return fsp_status;
}

void mac_address_conv_hex_str_to_char(char * input, uint8_t * output)
{
    uint8_t target = 0;
    char * addr_search = 0;
    char * addr_find = 0;

    addr_search = input;

    for(int i=0; i<5; i++){
        addr_find = strchr(addr_search, (int)':');
        *(output + target) += (uint8_t)(conv_hex_str_to_char(*(addr_find - 2)) * 16);
        *(output + target) += (uint8_t)(conv_hex_str_to_char(*(addr_find - 1)));

        target++;
        addr_search = addr_find + 1;
    }

    *(output + target) += (uint8_t)(conv_hex_str_to_char(*(addr_find + 1)) * 16);
    *(output + target) += (uint8_t)(conv_hex_str_to_char(*(addr_find + 2)));
}

uint8_t conv_hex_str_to_char(char input)
{
    uint8_t output = 0;

    if(('a' <= input) && (input <= 'f'))
    {
        output += (uint8_t)(input - 'a' + 10);
    }
    else if(('A' <= input) && (input <= 'F'))
    {
        output += (uint8_t)(input - 'A' + 10);
    }
    else if(('0' <= input) && (input <= '9'))
    {
        output += (uint8_t)(input - '0');
    }
    return output;
}

/*******************************************************************************************************************//**
 * @brief      Initialize IP through FreeRTOS_IPInit
 * @param[in]  void
 * @retval     None
 **********************************************************************************************************************/
BaseType_t network_ip_init(void)
{
    BaseType_t status = pdFALSE;

    /* FreeRTOS IP Initialization: This init initializes the IP stack  */
    status = FreeRTOS_IPInit (ucIPAddress, ucNetMask, ucGatewayAddress, ucDNSServerAddress, ucMACAddress);
    if (pdFALSE == status)
    {
        NETWORK_SETUP_APP_PRINT("FreeRTOS_IPInit Failed");
    }

    return status;
}

/*******************************************************************************************************************//**
 * @brief      Creates and prints  the IP configuration to display on the  console
 * @param[in]  void
 * @retval     None
 **********************************************************************************************************************/
void print_ipconfig(void)
{
#if( ipconfigUSE_DHCP != 0 )
    if (is_dhcp_in_use)
    {
        ucNetMask[3] = (uint8_t) ((xNd.ulNetMask & 0xFF000000) >> 24);
        ucNetMask[2] = (uint8_t) ((xNd.ulNetMask & 0x00FF0000) >> 16);
        ucNetMask[1] = (uint8_t) ((xNd.ulNetMask & 0x0000FF00) >> 8);
        ucNetMask[0] = (uint8_t) (xNd.ulNetMask & 0x000000FF);

        ucGatewayAddress[3] = (uint8_t) ((xNd.ulGatewayAddress & 0xFF000000) >> 24);
        ucGatewayAddress[2] = (uint8_t) ((xNd.ulGatewayAddress & 0x00FF0000) >> 16);
        ucGatewayAddress[1] = (uint8_t) ((xNd.ulGatewayAddress & 0x0000FF00) >> 8);
        ucGatewayAddress[0] = (uint8_t) (xNd.ulGatewayAddress & 0x000000FF);

        ucDNSServerAddress[3] = (uint8_t)((xNd.ulDNSServerAddresses[0] & 0xFF000000)>> 24);
        ucDNSServerAddress[2] = (uint8_t)((xNd.ulDNSServerAddresses[0] & 0x00FF0000)>> 16);
        ucDNSServerAddress[1] = (uint8_t)((xNd.ulDNSServerAddresses[0] & 0x0000FF00)>> 8);
        ucDNSServerAddress[0] = (uint8_t)(xNd.ulDNSServerAddresses[0] & 0x000000FF);

        ucIPAddress[3] = (uint8_t)((xNd.ulIPAddress & 0xFF000000) >> 24);
        ucIPAddress[2] = (uint8_t)((xNd.ulIPAddress & 0x00FF0000) >> 16);
        ucIPAddress[1] = (uint8_t)((xNd.ulIPAddress & 0x0000FF00) >> 8);
        ucIPAddress[0] = (uint8_t)(xNd.ulIPAddress & 0x000000FF);
    }
#endif
    NETWORK_SETUP_APP_PRINT("\r\nEthernet adapter for Renesas "KIT_NAME":\r\n");

    NETWORK_SETUP_APP_PRINT("\tDescription . . . . : Renesas "KIT_NAME" Ethernet\r\n");
    NETWORK_SETUP_APP_PRINT("\tPhysical Address. . : %02x-%02x-%02x-%02x-%02x-%02x\r\n", ucMACAddress[0], ucMACAddress[1], ucMACAddress[2], ucMACAddress[3], ucMACAddress[4], ucMACAddress[5]);
    NETWORK_SETUP_APP_PRINT("\tDHCP Enabled. . . . : %s\r\n", is_dhcp_in_use ? "Yes" : "No");
    NETWORK_SETUP_APP_PRINT("\tIPv4 Address. . . . : %d.%d.%d.%d\r\n", ucIPAddress[0], ucIPAddress[1], ucIPAddress[2], ucIPAddress[3]);
    NETWORK_SETUP_APP_PRINT("\tSubnet Mask . . . . : %d.%d.%d.%d\r\n", ucNetMask[0], ucNetMask[1], ucNetMask[2], ucNetMask[3]);
    NETWORK_SETUP_APP_PRINT("\tDefault Gateway . . : %d.%d.%d.%d\r\n", ucGatewayAddress[0], ucGatewayAddress[1], ucGatewayAddress[2], ucGatewayAddress[3]);
    NETWORK_SETUP_APP_PRINT("\tDNS Servers . . . . : %d.%d.%d.%d\r\n", ucDNSServerAddress[0], ucDNSServerAddress[1], ucDNSServerAddress[2], ucDNSServerAddress[3]);
}

/*******************************************************************************************************************//**
 * @brief      This Function checks the Network status (Both Ethernet and IP Layer). If the Network is down
 *             the Application will not send any data on the network.
 * @param[in]  None
 * @retval     Network Status
 **********************************************************************************************************************/
uint32_t isNetworkUp(void)
{
    fsp_err_t eth_link_status = FSP_ERR_NOT_OPEN;
    BaseType_t networkUp = pdFALSE;
    uint32_t network_status = (IP_LINK_UP | ETHERNET_LINK_UP);

    networkUp = FreeRTOS_IsNetworkUp ();
    eth_link_status = R_ETHER_LinkProcess (g_ether0.p_ctrl);

    if ((FSP_SUCCESS == eth_link_status) && (pdTRUE == networkUp))
    {
        return network_status;
    }
    else
    {
        if (FSP_SUCCESS != eth_link_status)
        {
            network_status |= ETHERNET_LINK_DOWN;
        }
        else if (FSP_SUCCESS == eth_link_status)
        {
            network_status |= ETHERNET_LINK_UP;
        }

        if (pdTRUE != networkUp)
        {
            network_status |= IP_LINK_DOWN;
        }
        else if (pdTRUE == networkUp)
        {
            network_status |= IP_LINK_UP;
        }

        return network_status;
    }
}

#elif (configENABLED_NETWORKS & AWSIOT_NETWORK_TYPE_WIFI)
#define WIFI_POSTINIT_INFO  "\r\n\r\n------------------------------------------------------------------------"\
                                "\r\n WiFi Interface Configuration "\
                                "\r\n------------------------------------------------------------------------\r\n"

dawifi_param_t g_dawifi;

static fsp_err_t wifi_init(void);
static fsp_err_t wifi_deinit(void);
static fsp_err_t wifi_connect(void);
static fsp_err_t network_connectivity_check(void);
static fsp_err_t wifi_preinit(void);

/***********************************************************************************************************************
 * @brief       This functions checks both WiFi connectivity and TCP socket connectivity.
 * @param[in]   None
 * @retval      fsp_err_t: FSP_SUCCESS or FSP_ERR_WIFI_AP_NOT_CONNECTED
 **********************************************************************************************************************/
fsp_err_t network_connectivity_check(void)
{
    fsp_err_t fsp_status = FSP_SUCCESS;

    if (pdTRUE != WIFI_IsConnected(NULL))
    {
        fsp_status = FSP_ERR_WIFI_AP_NOT_CONNECTED;
    }
    return fsp_status;
}

/*******************************************************************************************************************//**
 * @brief       This functions takes user  WiFi AP's SSID, Password and security type
 *              credentials. And connects to the SSID.
 * @param[IN]   None.
 * @retval      fsp_err_t: FSP_SUCCESS or FSP_ERR_WIFI_FAILED
 **********************************************************************************************************************/
fsp_err_t wifi_connect(void)
{
    fsp_err_t           fsp_status = FSP_SUCCESS;
    WIFINetworkParams_t pNwkParam  = {0x0};

    /* Set SSID, password and security type data to internal buffer */
    memcpy(&pNwkParam.ucSSID, &g_dawifi.ssid, strlen(g_dawifi.ssid));
    pNwkParam.ucSSIDLength = (uint8_t)strlen(g_dawifi.ssid);

    memcpy(&pNwkParam.xPassword.xWPA.cPassphrase, g_dawifi.pwd, strlen(g_dawifi.pwd));
    pNwkParam.xPassword.xWPA.ucLength = (uint8_t)strlen(g_dawifi.pwd);

    pNwkParam.xSecurity = g_dawifi.security;

    /* Connecting to AP via user entered SSID */
    if (eWiFiSuccess != WIFI_ConnectAP(&pNwkParam))
    {
        fsp_status = FSP_ERR_WIFI_FAILED;
    }
    return fsp_status;
}

/*******************************************************************************************************************//**
 * @brief       This functions validates and Iniitalizes the WiFi user credential settings.
 * @param[in]   None
 * @retval      fsp_err_t: FSP_SUCCESS or FSP_ERR_NOT_INITIALIZED
 **********************************************************************************************************************/
static fsp_err_t wifi_preinit(void)
{
    fsp_err_t fsp_status = FSP_SUCCESS;

    if ((0 == strlen (g_network_if_info)) || (eWiFiSecurityNotSupported == WIFI_SECURITY))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

#if (NETWORK_SETUP_USE_FIXED_INFO == 0U)
    char * p_string;

    /* Copy SSID and password data, which is stored in format <SSID> + <a space> + <password> */
    p_string = strtok(&g_network_if_info[0], " ");
    strcpy (g_dawifi.ssid, p_string);

    p_string = strtok(NULL, " ");
    strcpy (g_dawifi.pwd, p_string);
#else
    strcpy (g_dawifi.ssid, WIFI_SSID);
    strcpy (g_dawifi.pwd, WIFI_PASSWORD);
#endif

    g_dawifi.security = WIFI_SECURITY;

    return fsp_status;
}

/*******************************************************************************************************************//**
* @brief       This functions Iniitalizes the WiFi module.
* @param[in]   None
* @retval      fsp_err_t
**********************************************************************************************************************/
fsp_err_t wifi_init(void)
{
   fsp_err_t fsp_status = FSP_SUCCESS;

   fsp_status = wifi_preinit ();
   /* Handle error */
   if (FSP_SUCCESS != fsp_status)
   {
       return FSP_ERR_INVALID_ARGUMENT;
   }

   /* Opening WiFi module and Handle the Error*/
   if (eWiFiSuccess != WIFI_On())
   {
       return FSP_ERR_NOT_INITIALIZED;
   }
   return fsp_status;
}

/*******************************************************************************************************************//**
* @brief       This functions disconnects the socket connection and closes the WiFi module.
* @param[in]   None
* @retval      fsp_err_t
**********************************************************************************************************************/
fsp_err_t wifi_deinit(void)
{
   fsp_err_t err = FSP_SUCCESS;

   /* Checking socket connection status */
   err = network_connectivity_check ();
   if (FSP_SUCCESS != err)
   {
       NETWORK_SETUP_APP_PRINT("Wi-Fi is not connected.\r\n");
       return err;
   }

   if(eWiFiSuccess != WIFI_Disconnect ())
   {
       NETWORK_SETUP_APP_PRINT("[ERR] Wi-Fi disconnect failed.\r\n");
       return FSP_ERR_WIFI_FAILED;
   }

   /* Closing WiFi module. */
   /* Handle error */
   if (eWiFiSuccess != WIFI_Off())
   {
       NETWORK_SETUP_APP_PRINT("[ERR] Closing the Wi-Fi Module failed.\r\n");
       return FSP_ERR_WIFI_FAILED;
   }

   g_dawifi.status = !(FSP_SUCCESS);
   return err;
}
#endif

fsp_err_t network_interface_setup(void)
{
    fsp_err_t   fsp_status  = FSP_SUCCESS;

    NETWORK_SETUP_APP_PRINT("Network interface setup started...\r\n");

#if (configENABLED_NETWORKS & AWSIOT_NETWORK_TYPE_ETH)
    uint32_t                ip_status = RESET_VALUE;
    BaseType_t              bt_err = pdFALSE;
    uint8_t                 dns_retry_count = 0;

    fsp_status = network_ip_preinit();
    if(FSP_SUCCESS != fsp_status)
    {
        NETWORK_SETUP_APP_PRINT("[ERR] Wi-Fi credentials are invalid\r\n");
        return fsp_status;
    }

    /* Prints the Ethernet Configuration header prior to the IP Init. and prints the
     * Ethernet and IP configuration prior to IP Init. */
    NETWORK_SETUP_APP_PRINT(ETH_PREINIT);
    print_ipconfig();

    /* Network initialization: FreeRTOS IP Initialization; This initializes the IP stack  */
    bt_err = network_ip_init();
    if(pdTRUE == bt_err)
    {
        NETWORK_SETUP_APP_PRINT("FreeRTOS network IP init success\r\n");
    }
    else
    {
        NETWORK_SETUP_APP_PRINT("[ERR] FreeRTOS network IP init failed\r\n");
        return FSP_ERR_NOT_INITIALIZED;
    }

    NETWORK_SETUP_APP_PRINT("Waiting for network link up\r\n");
    bt_err = xTaskNotifyWait(pdFALSE, pdFALSE, &ip_status, portMAX_DELAY);
    if (pdTRUE != bt_err)
    {
        NETWORK_SETUP_APP_PRINT("[ERR] xTaskNotifyWait failed\r\n");
        return FSP_ERR_ETHER_ERROR_LINK;
    }

#if( ipconfigUSE_DHCP != 0 )
    NETWORK_SETUP_APP_PRINT("Waiting for DHCP IP address allocation complete\r\n");
    while(is_dhcp_complete == false)
    {
        vTaskDelay(1);
    }
#endif

    /* Prints the Ethernet Configuration Header for Post IP Init*/
    NETWORK_SETUP_APP_PRINT(ETH_POSTINIT);

    /* Get the IP address for the MQTT END POINT used for the application*/
    while(RESET_VALUE == ulIPAddress)
    {
        dns_retry_count++;

        ulIPAddress = FreeRTOS_gethostbyname((char*)g_mqtt_endpoint);

        if(RESET_VALUE == ulIPAddress)
        {
            NETWORK_SETUP_APP_PRINT("FreeRTOS_gethostbyname  Failed to get the End point address for %s\r\n",g_mqtt_endpoint);
        }

        if(dns_retry_count == 10)
        {
            if(RESET_VALUE == ulIPAddress)
            {
                return FSP_ERR_NOT_FOUND;
            }
        }
    }

    /* Convert the IP address to a string to print on to the console. */
    if(NULL == FreeRTOS_inet_ntoa( ulIPAddress, ( char * ) cBuffer))
    {
        APP_PRINT("\r\nDNS Lookup for \"%s\" is      : %s  \r\n", g_mqtt_endpoint,  cBuffer);
        return FSP_ERR_TIMEOUT;
    }
    print_ipconfig();
#endif
#if (configENABLED_NETWORKS & AWSIOT_NETWORK_TYPE_WIFI)
    WIFIIPConfiguration_t wifi_ipinfo = {0x0};

    /* WiFi Initialization */
    fsp_status = wifi_init();
    if (FSP_SUCCESS == fsp_status)
    {
        NETWORK_SETUP_APP_PRINT("Wi-Fi module open success\r\n");
    }
    else if(FSP_ERR_INVALID_ARGUMENT == fsp_status)
    {
        NETWORK_SETUP_APP_PRINT("[ERR] Wi-Fi credentials are invalid \r\n");
        return fsp_status;
    }
    else if(FSP_ERR_NOT_INITIALIZED == fsp_status)
    {
        NETWORK_SETUP_APP_PRINT("[ERR] Wi-Fi turn on failed\r\n");
        return fsp_status;
    }
    else
    {
        NETWORK_SETUP_APP_PRINT("[ERR] Wi-Fi module open failed\r\n");
        return fsp_status;
    }

    /* Connecting to user selected WiFi AP */
    NETWORK_SETUP_APP_PRINT("\r\nConnecting to SSID %s\r\n", g_dawifi.ssid);

    /* Connecting to WiFi AP/Router */
    fsp_status = wifi_connect();
    /* Handle error */
    if (FSP_SUCCESS != fsp_status)
    {
        NETWORK_SETUP_APP_PRINT("[ERR] Wi-Fi access point connect failed\r\n");

        if (FSP_SUCCESS == wifi_deinit())
        {
            NETWORK_SETUP_APP_PRINT("[ERR] Wi-Fi deinit failed\r\n");
        }
        return fsp_status;
    }

    /* Status check of WiFi connectivity */
    if (eWiFiSuccess != WIFI_IsConnected(NULL))
    {
        NETWORK_SETUP_APP_PRINT("[ERR] Wi-Fi channel not open\r\n");
        return FSP_ERR_INTERNAL;
    }

    NETWORK_SETUP_APP_PRINT("Wi-Fi connected to SSID %s\r\n",g_dawifi.ssid);

    /* Get and print assigned IP address */
    if(eWiFiSuccess != WIFI_GetIPInfo(&wifi_ipinfo))
    {
        NETWORK_SETUP_APP_PRINT("[ERR] IP address get failed.\r\n");
        fsp_status = FSP_ERR_WIFI_FAILED;

        if (FSP_SUCCESS != wifi_deinit ())
        {
            NETWORK_SETUP_APP_PRINT("[ERR] Wi-Fi deinit failed.\r\n");
        }
        return fsp_status;
    }
    g_dawifi.status = FSP_SUCCESS;


#if (NETWORK_WIFI_MODULE_TYPE == NETWORK_WIFI_MODULE_TYPE_DA16XXX)
    g_dawifi.device_ip_addr[0] = (uint8_t)(wifi_ipinfo.xIPAddress.ulAddress[0]);
    g_dawifi.device_ip_addr[1] = (uint8_t)(wifi_ipinfo.xIPAddress.ulAddress[0] >> 8);
    g_dawifi.device_ip_addr[2] = (uint8_t)(wifi_ipinfo.xIPAddress.ulAddress[0] >> 16);
    g_dawifi.device_ip_addr[3] = (uint8_t)(wifi_ipinfo.xIPAddress.ulAddress[0] >> 24);

    NETWORK_SETUP_APP_PRINT(WIFI_POSTINIT_INFO);
    NETWORK_SETUP_APP_PRINT("\tDescription . . . . : Renesas "KIT_NAME" + DA16200 Wi-Fi\r\n");
    NETWORK_SETUP_APP_PRINT("\tIPv4 Address  . . . : %3d.%3d.%3d.%3d\r\n",
                            g_dawifi.device_ip_addr[0],
                            g_dawifi.device_ip_addr[1],
                            g_dawifi.device_ip_addr[2],
                            g_dawifi.device_ip_addr[3]);
    NETWORK_SETUP_APP_PRINT("\tSubnet Mask   . . . : %3d.%3d.%3d.%3d\r\n",
                            (uint8_t)(wifi_ipinfo.xNetMask.ulAddress[0]),
                            (uint8_t)(wifi_ipinfo.xNetMask.ulAddress[0] >> 8),
                            (uint8_t)(wifi_ipinfo.xNetMask.ulAddress[0] >> 16),
                            (uint8_t)(wifi_ipinfo.xNetMask.ulAddress[0] >> 24));
    NETWORK_SETUP_APP_PRINT("\tGateway Address . . : %3d.%3d.%3d.%3d\r\n",
                            (uint8_t)(wifi_ipinfo.xGateway.ulAddress[0]),
                            (uint8_t)(wifi_ipinfo.xGateway.ulAddress[0] >> 8),
                            (uint8_t)(wifi_ipinfo.xGateway.ulAddress[0] >> 16),
                            (uint8_t)(wifi_ipinfo.xGateway.ulAddress[0] >> 24));
#elif (NETWORK_WIFI_MODULE_TYPE == NETWORK_WIFI_MODULE_TYPE_SILEX)
    g_dawifi.device_ip_addr[0] = (uint8_t)wifi_ipinfo.xIPAddress.ulAddress[0];
    g_dawifi.device_ip_addr[1] = (uint8_t)wifi_ipinfo.xIPAddress.ulAddress[1];
    g_dawifi.device_ip_addr[2] = (uint8_t)wifi_ipinfo.xIPAddress.ulAddress[2];
    g_dawifi.device_ip_addr[3] = (uint8_t)wifi_ipinfo.xIPAddress.ulAddress[3];

    NETWORK_SETUP_APP_PRINT(WIFI_POSTINIT_INFO);
    NETWORK_SETUP_APP_PRINT("\tDescription . . . . : Renesas "KIT_NAME" + Silex Wi-Fi\r\n");
    NETWORK_SETUP_APP_PRINT("\tIPv4 Address  . . . : %3d.%3d.%3d.%3d\r\n",
                            g_dawifi.device_ip_addr[0], g_dawifi.device_ip_addr[1], g_dawifi.device_ip_addr[2], g_dawifi.device_ip_addr[3]);
#endif
#endif

    NETWORK_SETUP_APP_PRINT("\r\nNetwork setup completed.\r\n");

    return FSP_SUCCESS;
}
