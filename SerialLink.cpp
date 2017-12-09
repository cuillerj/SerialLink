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
#include <Arduino.h>
#include <SerialLink.h>

SerialLink::SerialLink (unsigned int serialSpeed)
{
	_speedNetwSerial = serialSpeed;
	_active=false; // added the 19/11/2017
}

void SerialLink::SerialBegin(){
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
	#if defined(serial1)
		Serial1.begin(_speedNetwSerial);
	#endif
	#if defined(serial2)
		Serial2.begin(_speedNetwSerial);
	#endif
	#if defined(serial3)
		Serial3.begin(_speedNetwSerial);
	#endif
#else
		Serial.begin(_speedNetwSerial);
#endif
	_active=true; // added the 19/11/2017
}
void SerialLink::SerialEnd(){  // added the 19/11/2017
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
	#if defined(serial1)
		Serial1.end();
	#endif
	#if defined(serial2)
		Serial2.end();
	#endif
	#if defined(serial3)
		Serial3.end();
	#endif
#else
		Serial.end();
#endif
	_active=false; 
}
boolean SerialLink::SerialActive() // added the 19/11/2017
{
	return _active;
 }
void SerialLink::DataToSendSerial(){

 if (ConnectedSerial!=0x00 )
  {
	msgSerial[0]=0x7f;
    msgSerial[1]=0x7e;
	msgSerial[2]=0x7f;
    msgSerial[3]=0x7e;
    msgSerial[4]=addrSSerial;
    msgSerial[5]=addrMSerial;
    msgSerial[6]=PendingReqSerial;
 
	if (trySendSerial[1]==0x00)
	{
	pendingNumberSerial=pendingNumberSerial+1;
	trySendSerial[0]=pendingNumberSerial;
	trySendSerial[1]=0x00;
	}

    if (trySendSerial[0]!=pendingNumberSerial)
    {
      trySendSerial[0]=pendingNumberSerial;
      trySendSerial[1]=0x01;
    }
    else
    {
      trySendSerial[1]=trySendSerial[1]+1;
    }
	acqSerial=uint8_t(pendingNumberSerial);
    if (trySendSerial[1]<=1)   // 
    {
		msgSerial[7]=acqSerial;
		DataLenSerial=min(PendingDataLenSerial,sizeReqSerial);
		msgSerial[8]=uint8_t (DataLenSerial);

		for (int i=0;i<min(PendingDataLenSerial,sizeReqSerial);i++){
			msgSerial[i+9]=PendingDataReqSerial[i];

		}

#if defined(debugSerialLink)
		Serial.println();
		for (int i=0;i<9+DataLenSerial;i++)
		{
			Serial.print(msgSerial[i],HEX);
			Serial.print("-");
		}
		Serial.println();
#endif
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280_)
	#if defined(serial1)
		Serial1.write((uint8_t *)msgSerial,9+DataLenSerial);
	#endif
	#if defined(serial2)
		Serial2.write((uint8_t *)msgSerial,9+DataLenSerial);
	#endif
	#if defined(serial3)
		Serial3.write((uint8_t *)msgSerial,9+DataLenSerial);
	#endif
#else
		Serial.write((uint8_t *)msgSerial,9+DataLenSerial);
#endif
		tSendSerial=millis();
		trySendSerial[1]=0x01; // force ack recu avant eventuel developpement ack par esp
    }
    else
    {
      PendingReqSerial=0x00;
      PendingDataLenSerial=0;
      trySendSerial[0]=0x00;
      trySendSerial[1]=0x00;

    }
  }
}


uint8_t SerialLink::CheckUnitIndSerial()
{
	uint8_t retCode=0x00;
	retCode=PendingSecReqSerial;
  if (PendingSecReqSerial != 0){
    PendingReqSerial=PendingSecReqSerial;

	  for (int i=0;i<sizeof(sizeReqSerial);i++)
  {
	PendingDataReqSerial[i]=PendingSecDataReqSerial[i];
  }
    PendingDataLenSerial=PendingSecDataLenSerial;
  }

return retCode;
}

void SerialLink::SendSecuSerial(){

  PendingSecReqSerial=PendingReqSerial; // copy in case retry is needed
  for (int i=0;i<sizeof(sizeReqSerial-1);i++)
  {
	PendingSecDataReqSerial[i]=PendingDataReqSerial[i]; // copy in case retry is needed
  }

  PendingSecDataLenSerial=PendingDataLenSerial;  // copy in case retry is needed
}
void SerialLink::ReSendSecuSerial(){
	pendingNumberSerial=pendingNumberSerial-1;
    PendingReqSerial=PendingSecReqSerial;
	  for (int i=0;i<sizeof(sizeReqSerial-1);i++)
  {
	PendingDataReqSerial[i]=PendingSecDataReqSerial[i];
	  }
    PendingDataLenSerial=PendingSecDataLenSerial;
	trySendSerial[1]=0x01;
  }
  int SerialLink::Serial_have_message(){
	  int len=0;
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280_)
	#if defined(serial1)
	    if (Serial1.available() )
	#endif
	#if defined(serial2)
	    if (Serial2.available() )
	#endif
	#if defined(serial3)
	    if (Serial3.available() )
	#endif
#else
	    if (Serial.available() )
#endif
  {
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280_)
	#if defined(serial1)
    while (Serial1.available())
	#endif
	#if defined(serial2)
    while (Serial2.available())
	#endif
	#if defined(serial3)
    while (Serial3.available())
	#endif
#else
    while (Serial.available())
#endif
    {
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280_)
	#if defined(serial1)
      byte In1 = (Serial1.read());
	#endif
	#if defined(serial2)
      byte In1 = (Serial2.read());
	#endif
	#if defined(serial3)
      byte In1 = (Serial3.read());
	#endif
#else
      byte In1 = (Serial.read());	
#endif

			switch (In1){
				case 0x7f:
					if (frameFlag==0)
						{
							frameFlag=1;
							break;
						}
                    if (frameFlag==2)
                        {
                            frameFlag=3;
                            break;
						}
                    if (frameFlag==4)
                        {
                            frameFlag=5;
                            break;
						}
                     if (frameFlag==6)
                        {
                        frameFlag=7;
                        break;
                        }


				case 0x7e:
					if (frameFlag==3)
						{
							frameFlag=8;
							frameLen=0;
							frameCount=0;
							break;
						}
					if (frameFlag==1)
						{
                             frameFlag=2;

        						break;
						}
                    if (frameFlag==5)
                       {
                            frameFlag=6;
                            break;
                       }
                    if (frameFlag==7)
                       {
                            frameFlag=8;
                            frameLen=0;
                            frameCount=0;
                            break;
                        }

				default:


                    if (frameFlag==5 || frameFlag==6 || frameFlag==7 || frameFlag==8)
                        {
                            frameFlag=4;
                            break;
						}
			}

					switch (frameFlag)
						{

							case 4:  // useful data
							case 5:  // useful data
							case 6:  // useful data
 						frameCount=frameCount+1;


						switch (frameCount)
								{
									case 1:
										frameLen=In1;
										break;
																			
                                    default:
                           			DataInSerial[frameCount-2]=In1;
									if (frameCount==frameLen+1 )
									{

										len=frameLen;
									}
                                    break;
								}
						default:
						break;
						}

    }

  }
  return (len);
  }
  
// ***************************************************************************************************
