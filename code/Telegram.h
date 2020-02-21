#pragma once
struct Telegram
{
	int Sender;
	int Receiver;
	int Message;
	double DispatchTime;

	Telegram(double time, int sender, int receiver, int msg) : Sender(sender), Receiver(receiver), Message(msg), DispatchTime(time) { }
	Telegram() : Sender(-1), Receiver(-1), Message(-1), DispatchTime(-1) { }
};

