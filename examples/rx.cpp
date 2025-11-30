#include <cstdio>
#include "lora.h"
#include "packet.h"

#define SPI_BUS     1
#define SPI_CHANNEL 0
#define SS_PIN      27  // CE2, GPIO16
#define DIO0_PIN    21  // GPIO5, interrupt
#define RST_PIN     6  // GPIO25

int main() {
	printf("Setting up LoRa\n");
	LoRa lora(SPI_BUS, SPI_CHANNEL, SS_PIN, DIO0_PIN, RST_PIN);
	if (lora.begin()) {
		printf("LoRa setup successful: chipset version 0x%02x\n", lora.version());
		printf("Configuring radio\n");
                lora.setFrequency(869618000)
                        ->setTXPower(20)
                        ->setSpreadFactor(LoRa::SF_8)
                        ->setBandwidth(LoRa::BW_62k5)
                        ->setCodingRate(LoRa::CR_48)
                        ->setSyncWord(0x12)
                        ->setHeaderMode(LoRa::HM_EXPLICIT)
                        ->enableCRC()
			->setPreambleLength(16);
		printf("  TX power     : %d dB\n", lora.getTXPower());
		printf("  Frequency    : %d Hz\n", lora.getFrequency());
		printf("  Spread factor: %d\n", lora.getSpreadFactor());
		printf("  Bandwidth    : %d Hz\n", lora.bw[lora.getBandwidth()]);
		printf("  Coding Rate  : 4/%d\n", lora.getCodingRate() + 4);
		printf("  Sync word    : 0x%02x\n", lora.getSyncWord());
		printf("  Header mode  : %s\n", lora.getHeaderMode() == LoRa::HM_IMPLICIT ? "Implicit" : "Explicit");
		printf("Receiving...\n");
		while (true) {
			LoRaPacket p = lora.receivePacket();
			printf("Received packet\n");
			printf("  Bytes   : %d\n", p.payloadLength());
			printf("  RSSI    : %d dBm\n", p.getPacketRSSI());
			printf("  SNR     : %.1f dB\n", p.getSNR());
			printf("  Freq err: %d Hz\n", p.getFreqErr());
			printf("  Payload : \n%s\n", p.getPayload());
		}
	}
}
