/*
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Written by Jean Cuiller
*/
// Serial Link Arduino side
/* format du message
 ERCAMessK - E adresse emetteur sur 1 octet (HEX) R adresse recepteur -  C commande sur 1 octet (HEX) - A acquis/ reponse comande 1 octet- Mess Message(max 256 octets)(char) - longueur du message (hex)
 dans la phase d'init le master scrute le reseau a la recherche de stations dans la limite de MaxStations via le commande FF
 attend en retour FFAMessK (K = 1) Mess = type du recepteur 
 // numerique 3 5 6 utilise par virtuallib
 */
//  needed for RF434 network  >>
#ifndef SerialLink_h_included
#define SerialLink_h_included
#define sizeReqSerial 30    // define maximum data lengh
#define sizeInputSerial 30    // define maximum data lengh
#include <SerialLinkParameters.h>

//#define debugConnection true
#include <Arduino.h>
//typedef struct  {int length; char *Pdata;} getReturn;
class SerialLink
{
public:
SerialLink(unsigned int SpeedNetwSerial);
void DataToSendSerial();
void SendSecuSerial();
void ReSendSecuSerial();
void SerialBegin();
int Serial_have_message();
uint8_t CheckUnitIndSerial();
uint8_t addrSSerial=0x01; // station address par defaut
uint8_t addrMSerial=0x00;  // adresse Master 
uint8_t addrRSerial=0x00;  // destinataire
uint8_t cmdeSerial=0x00;  // commande (FF init comm, EF polling
uint8_t acqSerial=0x00;   // acq de la commande 
uint8_t dlenSerial=0x00;  // data length (start 6th Byte not longer than 30 Bytes)
uint8_t StationTypeSerial=0x00; // type of station 
uint8_t PendingReqSerial=0x00; // pending request (if 0x00 none)
uint8_t PendingDataReqSerial[sizeReqSerial]; // pending data request to send master
uint8_t PendingDataLenSerial;
uint8_t PendingSecReqSerial=0x00; // pending request (if 0x00 none) - copie pour retry
uint8_t PendingSecDataReqSerial[sizeReqSerial]; // pending data request to send master- copie pour retry
uint8_t PendingSecDataLenSerial;
uint8_t pendingNumberSerial=0x00;
uint8_t lastSentNumberSerial=0x00;
uint8_t trySendSerial[2]={0x00,0x00};
uint8_t lastReceivedNumberSerial=0x00;
uint8_t acqRSerial=0x00;
uint8_t frameFlag=0x00; 
uint8_t _serialNumber;
unsigned int _speedNetwSerial;
int frameLen=0;
int frameCount=0;
uint8_t bufParam[3];
//uint8_t PendingDataIn[10]; // pending data request 
int ConnectedSerial=1; // flag connexion force a 1 avant developpeme,t polling
int CheckReceiveSerial=100; // definit le cycle de check receive data sur RF434
unsigned long tSendSerial;
unsigned long tOutReceiveMasterSerial;
float Count1Serial; // zone de travail pour stat
float Count2Serial; // zone de travail pour stat
int DataLenSerial=0; // data input detected length
uint8_t msgSerial[sizeReqSerial+9]; // message in and out maxi 27 bytes including 0x7f+0x7e +0x7e+0x7f+header(5)
uint8_t DataInSerial[sizeInputSerial+9]; // data input (max 27)
};
#endif

// ***************************************************************************************************
