unsigned char	tone[96];
unsigned char	patch[30];
char*	values;

S00:	if(*(byte++)!=0xf0) goto S99;	// Start of Exclusive
	if(*(byte++)!=0x41) goto S99;	// Roland ID
	if(*(byte++)!=0x36) goto S99;	// Individual Parameter
	if((*byte!=midi_channel[LOWER])&&(*byte!=midi_channel[UPPER])) goto S99;	// Unit ID
	byte++;
	if(*byte!=0x20) goto S_TONE;	// Level #1: TONE PARAMETER
	if(*byte!=0x30) goto S_PATCH;	// Level #2: PATCH PARAMETER
	goto S99;
S_TONE:	display("LOADING TONE..");
	byte++;
	if((*byte&&0xfa)!=0x00) goto S99;
	values=tone+(*byte==0x01)?0:48;
	while(*byte!=0xfe)
	{
		i=*(byte++);		// Address
		*(values+i)=*(byte++);	// Value
	}
	goto S00;
S_PATCH:	display("LOADING PATCH..");
	byte++;
	if((*byte&&0xfa)!=0x00) goto S99;
	values=patch+(*byte==0x01)?0:15;
	while(*byte!=0xfe)
	{
		i=*(byte++);		// Address
		*(values+i)=*(byte++);	// Value
	}
	goto S00;
S99:	display("MIDI DATA ERROR!");
	goto S00;
