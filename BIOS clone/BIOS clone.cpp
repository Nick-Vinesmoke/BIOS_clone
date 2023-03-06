#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <deque>
using namespace std;

enum Borders { LEFT_MAIN = 0, RIGHT_MAIN = 91, TOP_MAIN = 1, BOTTOM_MAIN = 27, RIGHT_SIDE = 129 };
enum Main { LEFT_MAIN_IN = 1, RIGHT_MAIN_IN = 90, TOP_MAIN_IN = 2, BOTTOM_MAIN_IN = 26 };
enum Side { LEFT_SIDE_IN = 92, RIGHT_SIDE_IN = 128 };
enum Controls { TOP_CONTROLS_IN = 28, BOTTOM_BOTTOM_IN = 30 };
enum Colors {
	BLUE_BG_WHITE_TEXT = 23,
	GREY_BG_BLACK_TEXT = 112, GREY_BG_BLUE_TEXT = 121, GREY_BG_WHITE_TEXT = 127,
	CYAN_BG_BLACK_TEXT = 176, CYAN_BG_WHITE_TEXT = 191, DARKGREY_BG_WHITE_TEXT = 143
};
enum Keys {
	LEFT_ARROW = 75, F1 = 59, RIGHT_ARROW = 77, ENTER = 13,
	UP_ARROW = 72, DOWN_ARROW = 80, PLUS = 43, MINUS = 45
};
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

/// <summary>
/// Moving console handler around the console window.
/// </summary>
/// <param name="i">Needed X coord.</param>
/// <param name="j">Needed Y coord.</param>
void Goto(int i, int j) {
	SetConsoleCursorPosition(h, { (short)i, (short)j });
}
void print(int x, int y, int _char) {
	Goto(x, y);
	cout << unsigned char(_char);
}
void print(int _char) {
	cout << unsigned char(_char);
}
void hidecursor()
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(h, &info);
}

/// <summary>
/// Item of the menu class.
/// </summary>
class Item {
public:
	/// <summary>
	/// Name of the Item.
	/// </summary>
	string name;
	/// <summary>
	/// Index of the Item represents the number of the line on the screen from the top.
	/// </summary>
	int index;
	bool selected = false;
	deque<string>& options = options_;
	char type = 'i';
	char sub_menu = 0;
private:
	deque<string> options_;
public:
	/// <summary>
	/// Main class constructor.
	/// </summary>
	/// <param name="name">Desired name of the new Item.</param>
	/// <param name="i">Desired index of the new Item.</param>
	Item(string name, int i) : name(name), index(i) { AddOption("default"); }
	Item(string name, int i, deque<string> options) : name(name), index(i) {
		this->options = options;
	}
	virtual void Change(char dir) {
		if (dir == '+') {
			options.push_back(options.front());
			options.pop_front();
		}
		else if (dir == '-') {
			options.push_front(options.back());
			options.pop_back();
		}
	}
	virtual bool Editable() { return false; }
	virtual void Change_Sub() {};
	void AddOption(string value) {
		options.push_back(value);
	}
	/// <summary>
	/// Outputs a name of the Item from the top of the console + indent and offsets "default" parameter by 40 characters in the same line.
	/// </summary>
	/// <param name="indent">Desired indent from the top of the console screen.</param>
	virtual void Show(int indent) {
		if (selected)
		{
			SetConsoleTextAttribute(h, GREY_BG_WHITE_TEXT);
		}
		else {
			SetConsoleTextAttribute(h, GREY_BG_BLUE_TEXT);
		}
		Goto(LEFT_MAIN_IN + 1, index + indent);
		cout << name;
		Goto(LEFT_MAIN_IN + 40, index + indent);
		if (options.front() != " ")
		{
			cout << '[' << options.front() << ']';
		}
	}
};

class Time : public Item {
public:
	deque<string> op1;
	deque<string> op2;
	deque<string> op3;
	char isEdit = 0;

	Time(string name, int i, deque<string> op1, deque<string> op2, deque<string> op3) : Item(name, i) {
		this->op1 = op1;
		this->op2 = op2;
		this->op3 = op3;
		this->options = op1;
		this->type = 't';
		this->sub_menu = 1;
	}
	virtual void Change(char dir) {
		dir = 2 * (isEdit - 1) + dir;
		switch (dir)
		{
		case 1:
			op1.push_back(op1.front());
			op1.pop_front();
			break;
		case 2:
			op1.push_front(op1.back());
			op1.pop_back();
			break;
		case 3:
			op2.push_back(op2.front());
			op2.pop_front();
			break;
		case 4:
			op2.push_front(op2.back());
			op2.pop_back();
			break;
		case 5:
			op3.push_back(op3.front());
			op3.pop_front();
			break;
		case 6:
			op3.push_front(op3.back());
			op3.pop_back();
			break;
		default:
			break;
		}
	}
	virtual bool Editable() { return true; }
	virtual void Change_Sub() {
		isEdit++;
		if (isEdit == 4)
		{
			isEdit = 0;
		}
	}
	virtual void Show(int indent) {
		Goto(LEFT_MAIN_IN + 1, index + indent);
		if (selected)
		{
			SetConsoleTextAttribute(h, GREY_BG_WHITE_TEXT);
			cout << name;
			Goto(LEFT_MAIN_IN + 40, index + indent);
			if (isEdit != 0)
			{
				if (isEdit == 1)
				{
					SetConsoleTextAttribute(h, DARKGREY_BG_WHITE_TEXT);
					cout << op1.front();
					SetConsoleTextAttribute(h, GREY_BG_WHITE_TEXT);
					cout << '/' << op2.front() << '/' << op3.front();
				}
				else if (isEdit == 2) {

					SetConsoleTextAttribute(h, GREY_BG_WHITE_TEXT);
					cout << op1.front() << '/';
					SetConsoleTextAttribute(h, DARKGREY_BG_WHITE_TEXT);
					cout << op2.front();
					SetConsoleTextAttribute(h, GREY_BG_WHITE_TEXT);
					cout << '/' << op3.front();
				}
				else if (isEdit == 3) {
					SetConsoleTextAttribute(h, GREY_BG_WHITE_TEXT);
					cout << op1.front() << '/' << op2.front() << '/';
					SetConsoleTextAttribute(h, DARKGREY_BG_WHITE_TEXT);
					cout << op3.front();
				}
			}
			else {
				SetConsoleTextAttribute(h, GREY_BG_WHITE_TEXT);
				cout << op1.front() << '/' << op2.front() << '/' << op3.front();
			}
		}
		else {
			SetConsoleTextAttribute(h, GREY_BG_BLUE_TEXT);
			cout << name;
			Goto(LEFT_MAIN_IN + 40, index + indent);
			cout << op1.front() << '/' << op2.front() << '/' << op3.front();
		}
	}
};

/// <summary>
/// Top menu of the BIOS class.
/// </summary>
class Menu {
public:
	/// <summary>
	/// Displayed name of the manu.
	/// </summary>
	string name;
	/// <summary>
	/// IsSelected flag.
	/// </summary>
	bool selected = false;
	/// <summary>
	/// Items list.
	/// </summary>
	deque<Item*> items;

	Menu() : name("default"), items(deque<Item*>()), selected(false) {	}
	Menu(string name) {
		this->name = name;
		items = deque<Item*>();
	}

	void Setup(int index) {
		Goto(index, 0);
		if (selected)
		{
			SetConsoleTextAttribute(h, GREY_BG_BLUE_TEXT);
		}
		else {
			SetConsoleTextAttribute(h, BLUE_BG_WHITE_TEXT);
		}
		for (int i = 0; i < (15 - name.length()) / 2; i++) { cout << ' '; }
		cout << name;
		for (int i = 0; i < (15 - name.length()) / 2; i++) { cout << ' '; }
		SetConsoleTextAttribute(h, GREY_BG_BLUE_TEXT);
	}

	void Show() {
		SetConsoleTextAttribute(h, GREY_BG_BLUE_TEXT);
		for (int i = 0; i < items.size(); i++) {
			items[i]->Show((BOTTOM_MAIN_IN - TOP_MAIN_IN) / 2 - items.size() / 2);
		}
		SetConsoleTextAttribute(h, BLUE_BG_WHITE_TEXT);
	}
	void Push_item(string name, deque<string> options) {
		items.push_back(new Item(name, items.size(), options));
	}
	void Add_gap() {
		items.push_back(new Item(" ", items.size(), { " " }));
	}
	operator bool() { return name.length(); }
};

void ClearMain() {
	Goto(LEFT_MAIN_IN, TOP_MAIN_IN);
	for (int i = TOP_MAIN_IN; i < BOTTOM_MAIN_IN + TOP_MAIN_IN; i++)
	{
		for (int j = 0; j < RIGHT_MAIN_IN; j++)
		{
			cout << ' ';
		}
		Goto(LEFT_MAIN_IN, i);
	}
}
void ClearSide() {
	Goto(LEFT_SIDE_IN, TOP_MAIN_IN);
	for (int i = TOP_MAIN_IN; i < BOTTOM_MAIN_IN + TOP_MAIN_IN; i++)
	{
		for (int j = LEFT_SIDE_IN; j < RIGHT_SIDE_IN + 1; j++)
		{
			cout << ' ';
		}
		Goto(LEFT_SIDE_IN, i);
	}
}
void ClearControls() {
	SetConsoleTextAttribute(h, CYAN_BG_WHITE_TEXT);
	Goto(LEFT_MAIN, TOP_CONTROLS_IN);
	for (int i = TOP_CONTROLS_IN; i < BOTTOM_BOTTOM_IN + TOP_CONTROLS_IN; i++)
	{
		for (int j = LEFT_MAIN; j < RIGHT_SIDE + 1; j++)
		{
			cout << ' ';
		}
		Goto(LEFT_MAIN, i);
	}
}
void DrawBG() {
	SetConsoleTextAttribute(h, BLUE_BG_WHITE_TEXT);
	Goto(LEFT_MAIN, 0);
	for (int i = LEFT_MAIN; i < RIGHT_SIDE_IN + 2; i++) { cout << ' '; }
	SetConsoleTextAttribute(h, GREY_BG_BLUE_TEXT);
	ClearMain();
	ClearSide();
	ClearControls();
}
void DrawGrid() {
	SetConsoleTextAttribute(h, GREY_BG_BLACK_TEXT);
	print(LEFT_MAIN, TOP_MAIN, 218);
	print(RIGHT_MAIN, TOP_MAIN, 194);
	print(LEFT_MAIN, BOTTOM_MAIN, 192);
	print(RIGHT_MAIN, BOTTOM_MAIN, 193);
	print(RIGHT_SIDE, TOP_MAIN, 191);
	print(RIGHT_SIDE, BOTTOM_MAIN, 217);

	for (int i = 1; i < RIGHT_MAIN; i++) {
		print(i, TOP_MAIN, 196);
		print(i, BOTTOM_MAIN, 196);
	}
	for (int i = 1; i < RIGHT_SIDE - RIGHT_MAIN; i++)
	{
		print(i + RIGHT_MAIN, TOP_MAIN, 196);
		print(i + RIGHT_MAIN, BOTTOM_MAIN, 196);
	}
	for (int i = 2; i < BOTTOM_MAIN; i++) {
		print(LEFT_MAIN, i, 179);
		print(RIGHT_MAIN, i, 179);
		print(RIGHT_SIDE, i, 179);
	}
}
void DrawControls(string key, string description) {
	SetConsoleTextAttribute(h, CYAN_BG_WHITE_TEXT);
	cout << key << "\t";
	SetConsoleTextAttribute(h, CYAN_BG_BLACK_TEXT);
	cout << description;
}
void UpdateControls() {
	Goto(130 / 4 * 0, TOP_CONTROLS_IN);
	DrawControls("   F1", "Help");
	Goto(130 / 4 * 0, TOP_CONTROLS_IN + 1);
	DrawControls("   ESC", "Exit");

	Goto(130 / 4 * 1, TOP_CONTROLS_IN);
	DrawControls("UP/DOWN  ", "Select Item");
	Goto(130 / 4 * 1, TOP_CONTROLS_IN + 1);
	DrawControls("LEFT/RIGHT", "Exit");

	Goto(130 / 4 * 2, TOP_CONTROLS_IN);
	DrawControls("+/-", "Change Values");
	Goto(130 / 4 * 2, TOP_CONTROLS_IN + 1);
	DrawControls("Enter", "Select > Sub Menu");

	Goto(130 / 4 * 3, TOP_CONTROLS_IN);
	DrawControls("F9", "Setup Defaults");
	Goto(130 / 4 * 3, TOP_CONTROLS_IN + 1);
	DrawControls("F10", "Save and Exit");
}

deque<Menu> menues;

void UpdateHeadMenu() {
	SetConsoleTextAttribute(h, GREY_BG_BLACK_TEXT);
	int n = 6;
	for (int i = 0; i < n; i++) {
		menues[i].Setup(i * 15);
	}
}

int main()
{
	system("chcp 437");
	system("cls");
	hidecursor();
	DrawBG();

	menues.push_back(Menu("Main"));
	menues.push_back(Menu("Advanced"));
	menues.push_back(Menu("Security"));
	menues.push_back(Menu("Power"));
	menues.push_back(Menu("Boot"));
	menues.push_back(Menu("Exit"));

	menues[0].selected = true;

	deque<string> hour = deque<string>();
	for (int i = 0; i < 24; i++) { hour.push_back(to_string(i)); }
	deque<string> minutes = deque<string>();
	for (int i = 0; i < 60; i++) { minutes.push_back(to_string(i)); }
	deque<string> seconds = deque<string>();
	for (int i = 0; i < 60; i++) { seconds.push_back(to_string(i)); }
	menues[0].items.push_back(new Time("System Time", 0, hour, minutes, seconds));

	deque<string> day = deque<string>();
	for (int i = 1; i < 32; i++) { day.push_back(to_string(i)); }
	deque<string> month = deque<string>();
	for (int i = 1; i < 13; i++) { month.push_back(to_string(i)); }
	deque<string> year = deque<string>();
	for (int i = 1999; i < 2020; i++) { year.push_back(to_string(i)); }
	menues[0].items.push_back(new Time("System Date", 1, day, month, year));

	menues[0].Push_item("Legacy Diskette A",
		{ "None",
		"360K, 5.25 in.",
		"1.2M, 5.25 in.",
		"720K, 3.5 in.",
		"1.44M, 3.5 in.",
		"2.88M, 3.5 in."
		});
	menues[0].Push_item("Legacy Diskette B",
		{ "None",
		"360K, 5.25 in.",
		"1.2M, 5.25 in.",
		"720K, 3.5 in.",
		"1.44M, 3.5 in.",
		"2.88M, 3.5 in."
		});
	menues[0].Push_item("Floppy 3 Mode Support", { "Disabled", "Enabled" });

	menues[1].Push_item("CPU Speed", { "1400 Mhz" });
	menues[1].Push_item("CPU: System Frequency Multiple", { "12.0x" });
	menues[1].Push_item("System/PCI Frequency (Mhz)", { "133/33" });
	menues[1].Push_item("System /SDRAM Frequency Ratio", { "Auto" });
	menues[1].Push_item("Load Performance Setting", { "Normal" });
	menues[1].Push_item("CPU Vcore", { "1.750V" });
	menues[1].Push_item("CPU Level 1 Cache", { "Enabled" });
	menues[1].Push_item("CPU Level 2 Cache", { "Enabled" });
	menues[1].Push_item("CPU Level 2 Cache ECC Check", { "Disabled" });
	menues[1].Push_item("BIOS Update", { "Enabled" });
	menues[1].Push_item("PS/2 Mouse Function Control", { "Auto" });
	menues[1].Push_item("USB Legacy Support", { "Auto" });
	menues[1].Push_item("OS/2 Onboard Memory > 64 M", { "Disabled" });
	menues[1].Push_item("Language", { "English" });
	menues[1].Push_item("Installed Memory", { "768 MB" });
	menues[1].Push_item("Halt On",
		{ "All Errors",
		"No Error",
		"All but Keyboard",
		"All but Disk",
		"All but Disk / Keyboard"
		});

	menues[2].Push_item("Supervisor Password", { "Disabled" });
	menues[2].Push_item("User Password", { "Disabled" });

	menues[3].Push_item("Power Management", { "User Define" });
	menues[3].Push_item("Video Off Option", { "Suspend -> Off" });
	menues[3].Push_item("Video Off Method", { "DPMS OFF" });
	menues[3].Push_item("HDD Power Down", { "Disabled" });
	menues[3].Push_item("Suspend-RAM Capability", { "Disabled" });
	menues[3].Add_gap();
	menues[3].Push_item("Suspend Mode", { "Disabled" });
	menues[3].Push_item("PWR Button < 4 Secs", { "Soft Off" });
	menues[3].Push_item("CPU Thermal Option", { "Disabled" });

	menues[4].Push_item("1. IDE Hard Drive", { "None", "IC35L040AVVA07-0" });
	menues[4].Push_item("2. ATAPI CD-ROM", { "None", "PLEXTOR PX-W4012TA" });
	menues[4].Push_item("3. Removable Device", { "None", "Legacy Floppy" });
	menues[4].Push_item("4. Other Boot Device", { "Disabled" });
	menues[4].Add_gap();
	menues[4].Push_item("Plug & Play O/S", { "Yes" });
	menues[4].Push_item("Reset Configuration Data", { "No" });
	menues[4].Push_item("Boot Virus Detection", { "Enabled" });
	menues[4].Push_item("Quick Power On Self Test", { "Enabled" });
	menues[4].Push_item("Boot Up Floppy Seek", { "Disabled" });

	menues[5].Push_item("Exit Saving Changes", { " " });
	menues[5].Push_item("Exit Discarding Changes", { " " });
	menues[5].Push_item("Load Setup Defaults", { " " });
	menues[5].Push_item("Discard Changes", { " " });
	menues[5].Push_item("Save Changes", { " " });

	UpdateHeadMenu();
	UpdateControls();
	DrawGrid();
	int m = 0;
	menues[0].Show();
	Goto(menues[0].name.length(), 0);
	int selected_menu = 0;
	int selected_item = -1;
	while (true) {
		int keycode = _getch();
		if (keycode == RIGHT_ARROW)
		{
			if (selected_item != -1)
			{
				menues[selected_menu].items[selected_item]->selected = false;
				selected_item = -1;
			}

			if (selected_menu < 5)
			{
				selected_menu++;
				menues[selected_menu - 1].selected = false;
				menues[selected_menu].selected = true;
			}
		}
		else if (keycode == LEFT_ARROW) {
			if (selected_item != -1)
			{
				menues[selected_menu].items[selected_item]->selected = false;
				selected_item = -1;
			}
			if (selected_menu > 0)
			{
				selected_menu--;
				menues[selected_menu + 1].selected = false;
				menues[selected_menu].selected = true;
			}
		}
		else if (keycode == DOWN_ARROW) {
			if (selected_item < (int)menues[selected_menu].items.size() - 1)
			{
				selected_item++;
				if (selected_item == 0)
				{
					menues[selected_menu].items[selected_item]->selected = true;
				}
				else {
					menues[selected_menu].items[selected_item - 1]->selected = false;
					menues[selected_menu].items[selected_item]->selected = true;
				}
			}
		}
		else if (keycode == UP_ARROW) {
			if (selected_item > 0) {
				selected_item--;
				if (selected_item == menues[selected_menu].items.size() - 1)
				{
					menues[selected_menu].items[selected_item]->selected = true;
				}
				else {
					menues[selected_menu].items[selected_item + 1]->selected = false;
					menues[selected_menu].items[selected_item]->selected = true;
				}
			}
		}
		else if (keycode == PLUS) {
			if (selected_item >= 0) {
				if (menues[selected_menu].items[selected_item]->Editable()) {
					menues[selected_menu].items[selected_item]->Change(1);
				}
				else if (menues[selected_menu].items[selected_item]->type == 'i')
				{
					menues[selected_menu].items[selected_item]->Change('+');
				}
			}
		}
		else if (keycode == MINUS) {
			if (selected_item >= 0) {
				if (menues[selected_menu].items[selected_item]->Editable()) {
					menues[selected_menu].items[selected_item]->Change(2);
				}
				else if (menues[selected_menu].items[selected_item]->type == 'i')
				{
					menues[selected_menu].items[selected_item]->Change('-');
				}
			}
		}
		else if (keycode == ENTER) {
			if (menues[selected_menu].items[selected_item]->Editable())
			{
				menues[selected_menu].items[selected_item]->Change_Sub();
			}
		}
		ClearMain();
		menues[selected_menu].Show();
		UpdateHeadMenu();
		DrawGrid();
		/*cout << keycode;
		cin.get();*/
	}

}