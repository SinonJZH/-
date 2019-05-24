#include<iostream>
#include<algorithm>
#include<string>
#include<list>
#include<fstream>
#include<ctime>
#include<cstdlib>
#include<Windows.h>
#include<iomanip>
#include<map>

#define PASSWD "test"

using namespace std;

typedef struct MEDI
{
	string name, type;
	long long in_stack, sell_amount;
	time_t produ_time, due_time;

	bool operator <(const MEDI &x)const
	{
		return sell_amount > x.sell_amount;
	}
}medi;//单个药品结构体

int menu();
int login();
int add();
int search();
int change();
int del();
int due();
int cal();
int sim();
int Exit();
int file();

int main()
{
	int command;
	login();
	system("cls");
	file();
	system("cls");
	while (1)
	{
		command=menu();
		switch (command)
		{
		case 1:add(); break;
		case 2:search(); break;
		case 3:change(); break;
		case 4:del(); break;
		case 5:due(); break;
		case 6:cal(); break;
		case 7:sim(); break;
		case 8:Exit(); break;
		}
		system("cls");//清屏
	}
	return 0;
}

int login()
{
	cout << "欢迎使用药房销售系统，请先登录，或输出exit退出：\n";
	string a;
	while (1)
	{
		cin >> a;
		if (!a.compare(PASSWD))//字符串比较
		{
			break;
		}
		else if (!a.compare("exit"))
		{
			exit(0);
		}
		cout << "密码错误，请重试！";
		Sleep(1000);
		system("cls");
	}
	return 0;
}

int add()
{
	fstream dat;
	time_t time1, time2;
	long long due_day;
	struct tm *p = new struct tm;
	int y, m, d;
	dat.open("data.dat", ios::app|ios::out);//在文件末尾追加写入
	time(&time1);//将当前时间戳保存在time1中
	localtime_s(p, &time1);//把时间戳转换为tm结构体保存在p中
	medi add_item;
	cout << "请输入药品名称：";
	cin >> add_item.name;
	cout << "请输入药品类别：";
	cin >> add_item.type;
	cout << "请输入生产日期(年 月 日)(使用空格分隔)";
	cin >> y >> m >> d;
	while (1)
	{
		if (y <= 1900 || y >= 2100 || m <= 0 || m > 12 || d <= 0 || d > 31)
		{
			cout << "输入的日期非法！请重新输入！\n";
			cout << "请输入生产日期(年 月 日)(使用空格分隔)";
			cin >> y >> m >> d;
			cin.clear();//清除cin错误
			cin.ignore(1, '\n');//清除输入缓冲区
		}
		else
		{
			p->tm_year = y - 1900;
			p->tm_mon = m - 1;
			p->tm_mday = d;
			p->tm_hour = 0;
			p->tm_sec = 0;
			time2 = mktime(p);
			if (time2 > time1)
			{
				cout << "生产日期超过当前日期！请重新输入！\n";
				cout << "请输入生产日期(年 月 日)(使用空格分隔)";
				cin >> y >> m >> d;
			}
			else
			{
				break;
			}
		}
	}
	add_item.produ_time = time2;
	cout << "请输入保质期(天)：";
	cin >> due_day;
	while (1)
	{
		if (due_day <= 0 || due_day >= 3600)
		{
			cout << "保质期非法！请重新输入！\n";
			cin >> due_day;
			cin.clear();
			cin.ignore(1, '\n');
		}
		else
		{
			break;
		}
	}
	time1 = time2 + due_day * 86400;
	add_item.due_time = time1;
	cout << "请输入库存：";
	cin >> due_day;//再利用due_day,作为输入库存值
	while (1)
	{
		if (due_day < 0)
		{
			cout << "输入的库存数量非法!请重新输入！\n";
			cout << "请输入库存：";
			cin >> due_day;
			cin.clear();
			cin.ignore(1, '\n');
		}
		else
		{
			break;
		}
	}
	add_item.in_stack = due_day;
	dat << add_item.name << ' ' << add_item.type << ' ' << add_item.produ_time << ' ' << add_item.due_time << ' ' << add_item.in_stack << ' ' << 0 << endl;
	cout << "数据添加成功！";
	Sleep(1000);
	dat.close();
	return 0;
}

int search()
{
	int cmd;
	long long counter = 0, out_counter = 0, output[100], cache[100][100] = { 0 }, id;
	fstream dat;
	medi data[100];
	string keyword;
	map<string, long long>mp;
	map<string, long long>::iterator n;
	dat.open("data.dat", ios::in);//打开文件读取
	while (!dat.eof())//读取文件直到末尾
	{
		dat >> data[counter].name >> data[counter].type >> data[counter].produ_time >> data[counter].due_time >> data[counter].in_stack >> data[counter].sell_amount;
		counter++;
	}
	dat.close();
	if (counter == 0)
	{
		cout << "数据库中还没有添加过药品！";
		Sleep(1000);
		return 0;
	}
	counter--;
	cout<<"请选择查询模式：(1.全部显示/2.按名称查找/3.按类别查找)";
	cin >> cmd;
	if (cmd == 1)
	{
		cout << setw(44) << ' ' << "全部药品清单" << setw(44) << ' ' << setw(0) << endl;
		cout << "  id               名称                      生产日期            到期日期           库存数量          累计销量     \n";
		for (long long i = 0; i < counter; i++)
		{
			struct tm *p1 = new struct tm, *p2 = new struct tm;
			char pt[100], dt[100];
			localtime_s(p1, &data[i].produ_time);
			strftime(pt, 100, "%Y年%m月%d日", p1);//把时间格式化成想要的格式用于输出
			localtime_s(p2, &data[i].due_time);
			strftime(dt, 100, "%Y年%m月%d日", p2);
			cout << setw(6) << i << setw(30) << data[i].name << setw(20) << pt << setw(20) << dt << setw(18) << data[i].in_stack << setw(18) << data[i].sell_amount << setw(0) << endl;
		}	
	}
	else if (cmd == 2)
	{
		cout << "请输入搜索关键字：";
		cin >> keyword;
		cout << setw(46) << ' ' << "搜索结果" << setw(46) << ' ' << setw(0) << endl;
		cout << "  id               名称                      生产日期            到期日期           库存数量          累计销量     \n";
		for (int i = 1; i < counter; i++)
		{
			if (data[i].name.find(keyword) != string::npos)
			{
				output[out_counter] = i;
				out_counter++;
			}
		}
		if (out_counter == 0)
		{
			cout << "未检索到相关药品！";
			Sleep(1000);
			return 0;
		}
		for (long long i = 0; i < out_counter; i++)
		{
			id = output[i];
			struct tm *p1 = new struct tm, *p2 = new struct tm;
			char pt[100], dt[100];
			localtime_s(p1, &data[id].produ_time);
			strftime(pt, 100, "%Y年%m月%d日", p1);//把时间格式化成想要的格式用于输出
			localtime_s(p2, &data[id].due_time);
			strftime(dt, 100, "%Y年%m月%d日", p2);
			cout << setw(6) << id << setw(30) << data[id].name << setw(20) << pt << setw(20) << dt << setw(18) << data[id].in_stack << setw(18) << data[id].sell_amount << setw(0) << endl;
		}
	}
	else if (cmd == 3)
	{
		cout << "**************所有分类**************\n";
		for (long long i = 0; i < counter; i++)
		{
			if (!mp.count(data[i].type))
			{
				mp[data[i].type] = out_counter;
				cache[out_counter][0] = 1;
				cache[out_counter][1] = i;
				out_counter++;
			}
			else
			{
				long long tmp = mp[data[i].type];
				id = cache[tmp][0] + 1;
				cache[tmp][0]++;
				cache[tmp][id] = i;
			}
		}
		n = mp.end();
		n--;
		for (n ; n != mp.begin(); n--)
		{
			cout << '*' << setw(5) << n->second << '.' << setw(28) << n->first << '*' << endl;
		}
		cout << '*' << setw(5) << n->second << '.' << setw(28) << n->first << '*' << endl;
		cout << "************************************\n";
		cout << "输入分类前的数字序号查看该分类下所有药品：";
		cin >> id;
		if (id < 0 || id >= out_counter)
		{
			cout << "输入错误!";
			Sleep(1000);
			return 0;
		}
		out_counter = cache[id][0];
		cout << setw(46) << ' ' << "搜索结果" << setw(46) << ' ' << setw(0) << endl;
		cout << "  id               名称                      生产日期            到期日期           库存数量          累计销量     \n";
		for (long long i = 1; i <= out_counter; i++)
		{
			long long tmp = cache[id][i];
			struct tm *p1 = new struct tm, *p2 = new struct tm;
			char pt[100], dt[100];
			localtime_s(p1, &data[tmp].produ_time);
			strftime(pt, 100, "%Y年%m月%d日", p1);//把时间格式化成想要的格式用于输出
			localtime_s(p2, &data[tmp].due_time);
			strftime(dt, 100, "%Y年%m月%d日", p2);
			cout << setw(6) << tmp << setw(30) << data[tmp].name << setw(20) << pt << setw(20) << dt << setw(18) << data[tmp].in_stack << setw(18) << data[tmp].sell_amount << setw(0) << endl;
		}
	}
	system("pause");
	return 0;
}

int change()
{
	long long counter = 0, id;
	int cmd, y, m, d;
	fstream dat;
	struct tm *p1 = new struct tm, *p2 = new struct tm;
	time_t time1, time2;
	medi data[100];
	dat.open("data.dat", ios::in);
	while (!dat.eof())
	{
		dat >> data[counter].name >> data[counter].type >> data[counter].produ_time >> data[counter].due_time >> data[counter].in_stack >> data[counter].sell_amount;
		counter++;
	}
	dat.close();
	counter--;
	if (counter == 0)
	{
		cout << "数据库中还没有添加过药品！";
		Sleep(1000);
		return 0;
	}
	cout << "请输入需要修改的药品id:";
	cin >> id;
	if (id<0 || id>=counter)
	{
		cout << "无此id！";
		Sleep(1000);
		return 0;
	}
	while (1)
	{
		char pt[100], dt[100];
		localtime_s(p1, &data[id].produ_time);
		strftime(pt, 100, "%Y年%m月%d日", p1);//把时间格式化成想要的格式用于输出
		localtime_s(p2, &data[id].due_time);
		strftime(dt, 100, "%Y年%m月%d日", p2);
		system("cls");
		cout << "********************药品信息修改********************\n"
			<< "*1.药品名称：" << setw(38) << data[id].name << setw(0) << "*\n"//setw(int x)把输出宽度设置为x
			<< "*2.药品类别：" << setw(38) << data[id].type << setw(0) << "*\n"
			<< "*3.生产日期：" << setw(38) << pt << setw(0) << "*\n"
			<< "*4.保质期至：" << setw(38) << dt << setw(0) << "*\n"
			<< "*5.库存数量：" << setw(38) << data[id].in_stack << setw(0) << "*\n"
			<< "*6.累计销量：" << setw(38) << data[id].sell_amount << setw(0) << "*\n"
			<< "*7.保存并退出                                      *\n"
			<< "*********输入需要修改的项目前序号进行修改***********\n";
		cin >> cmd;
		cin.clear();
		cin.ignore(1, '\n');
		if (cmd == 1)
		{
			cout << "请输入新的药品名称：";
			cin >> data[id].name;
		}
		else if (cmd == 2)
		{
			cout << "请输入新的药品类别：";
			cin >> data[id].type;
		}
		else if (cmd == 3)
		{
			cout << "请输入新的生产日期(年 月 日)(使用空格分隔):";
			cin >> y >> m >> d;
			while (1)
			{
				if (y <= 1900 || y >= 2100 || m <= 0 || m > 12 || d <= 0 || d > 31)
				{
					cout << "输入的日期非法！请重新输入！\n";
					cout << "请输入生产日期(年 月 日)(使用空格分隔)";
					cin >> y >> m >> d;
					cin.clear();
					cin.ignore(1, '\n');
				}
				else
				{
					p1->tm_year = y - 1900;
					p1->tm_mon = m - 1;
					p1->tm_mday = d;
					time2 = mktime(p1);
					time(&time1);
					if (time2 > time1)
					{
						cout << "生产日期超过当前日期！请重新输入！\n";
						cout << "请输入生产日期(年 月 日)(使用空格分隔)";
						cin >> y >> m >> d;
					}
					else
					{
						break;
					}
				}
			}
			data[id].produ_time = time2;
		}
		else if (cmd == 4)
		{
			cout << "请输入新的过期日期(年 月 日)(使用空格分隔):";
			cin >> y >> m >> d;
			while (1)
			{
				if (y <= 1900 || y >= 2100 || m <= 0 || m > 12 || d <= 0 || d > 31)
				{
					cout << "输入的日期非法！请重新输入！\n";
					cout << "请输入过期日期(年 月 日)(使用空格分隔)";
					cin >> y >> m >> d;
					cin.clear();
					cin.ignore(1, '\n');
				}
				else
				{
					p1->tm_year = y - 1900;
					p1->tm_mon = m - 1;
					p1->tm_mday = d;
					time2 = mktime(p1);
					if (time2 < data[id].produ_time)
					{
						cout << "过期日期早于生产日期！请重新输入！\n";
						cout << "请输入过期日期(年 月 日)(使用空格分隔)";
						cin >> y >> m >> d;
					}
					else
					{
						break;
					}
				}
			}		
			data[id].due_time = time2;
		}
		else if (cmd == 5)
		{
			cout << "请输入新的库存：";
			cin >> data[id].in_stack;
			cin.clear();
			cin.ignore(1, '\n');
		}
		else if (cmd == 6)
		{
			cout << "请输入新的销量：";
			cin >> data[id].sell_amount;
			cin.clear();
			cin.ignore(1, '\n');
		}
		else if (cmd == 7)
		{
			dat.open("data.dat", ios::out | ios::trunc);
			for (long long i = 0; i < counter; i++)
			{
				dat << data[i].name << ' ' << data[i].type << ' ' << data[i].produ_time << ' ' << data[i].due_time << ' ' << data[i].in_stack << ' ' << data[i].sell_amount << endl;
			}
			dat.close();
			cout << "修改成功！";
			Sleep(1000);
			return 0;
		}
	}
	return 0;
}

int del()
{
	long long id, counter = 0;
	fstream dat;
	medi data[100];
	dat.open("data.dat", ios::in);//打开文件读取
	while (!dat.eof())//读取文件直到末尾
	{
		dat >> data[counter].name >> data[counter].type >> data[counter].produ_time >> data[counter].due_time >> data[counter].in_stack >> data[counter].sell_amount;
		counter++;
	}
	dat.close();
	counter--;
	if (counter == 0)
	{
		cout << "数据库中还没有添加过药品！";
		Sleep(1000);
		return 0;
	}
	cout << "请输入需要删除的药品id:";
	cin >> id;
	if (id < 0 || id >= counter)
	{
		cout << "无此id！";
		Sleep(1000);
		return 0;
	}
	dat.open("data.dat", ios::out | ios::trunc);//从头写入文件并将文件清空
	for (long long i = 0; i < counter; i++)
	{
		if (i == id)continue;
		dat << data[i].name << ' ' << data[i].type << ' ' << data[i].produ_time << ' ' << data[i].due_time << ' ' << data[i].in_stack << ' ' << data[i].sell_amount << endl;
	}
	dat.close();
	cout << "删除成功！";
	Sleep(1000);
	return 0;
}

int due()
{
	int cmd;
	fstream dat;
	long long id, counter = 0, out_counter = 0;
	medi data[100];
	long long output[100];
	time_t now;
	cout << "请选择显示模式：(1.已过期/2.即将过期)";
	cin >> cmd;
	cin.clear();
	cin.ignore(1, '\n');
	if (cmd <= 0 || cmd > 2)
	{
		cout << "输入非法！";
		return 0;
	}
	dat.open("data.dat", ios::in);
	while (!dat.eof())
	{
		dat >> data[counter].name >> data[counter].type >> data[counter].produ_time >> data[counter].due_time >> data[counter].in_stack >> data[counter].sell_amount;
		counter++;
	}
	dat.close();
	counter--;
	if (counter == 0)
	{
		cout << "数据库中还没有添加过药品！";
		Sleep(1000);
		return 0;
	}
	if (cmd == 1)
	{
		time(&now);
		for (long long i = 0; i < counter; i++)
		{
			if (data[i].due_time < now)
			{
				output[out_counter] = i;
				out_counter++;
			}
		}
		if (out_counter == 0)
		{
			cout << "还没有药品过期！";
			Sleep(1000);
			return 0;
		}
		cout << setw(44) << ' '<< "过期药品清单" << setw(44) << ' ' << setw(0) << endl;
		cout << "  id               名称                      生产日期            到期日期           库存数量          累计销量     \n";
		for (long long i = 0; i < out_counter; i++)
		{
			id = output[i];
			struct tm *p1 = new struct tm, *p2 = new struct tm;
			char pt[100], dt[100];
			localtime_s(p1, &data[id].produ_time);
			strftime(pt, 100, "%Y年%m月%d日", p1);//把时间格式化成想要的格式用于输出
			localtime_s(p2, &data[id].due_time);
			strftime(dt, 100, "%Y年%m月%d日", p2);
			cout << setw(6) << id << setw(30) << data[id].name << setw(20) << pt << setw(20) << dt << setw(18) << data[id].in_stack << setw(18) << data[id].sell_amount << setw(0) << endl;
		}
		system("pause");
	}
	else if (cmd == 2)
	{
		time(&now);
		for (long long i = 0; i < counter; i++)
		{
			if ((data[i].due_time - now)<=2592000 && (data[i].due_time - now)>=0)
			{
				output[out_counter] = i;
				out_counter++;
			}
		}
		if (out_counter == 0)
		{
			cout << "还没有药品即将过期！";
			Sleep(1000);
			return 0;
		}
		cout << setw(44) << ' ' << "即期药品清单" << setw(44) << ' ' << setw(0) << endl;
		cout << "  id               名称                      生产日期            到期日期           库存数量          累计销量     \n";
		for (long long i = 0; i < out_counter; i++)
		{
			id = output[i];
			struct tm *p1 = new struct tm, *p2 = new struct tm;
			char pt[100], dt[100];
			localtime_s(p1, &data[id].produ_time);
			strftime(pt, 100, "%Y年%m月%d日", p1);//把时间格式化成想要的格式用于输出
			localtime_s(p2, &data[id].due_time);
			strftime(dt, 100, "%Y年%m月%d日", p2);
			cout << setw(6) << id << setw(30) << data[id].name << setw(20) << pt << setw(20) << dt << setw(18) << data[id].in_stack << setw(18) << data[id].sell_amount << setw(0) << endl;
		}
		system("pause");
	}
	return 0;
}

int cal()
{
	long long counter = 0;
	fstream dat;
	medi data[100];
	dat.open("data.dat", ios::in);//打开文件读取
	while (!dat.eof())//读取文件直到末尾
	{
		dat >> data[counter].name >> data[counter].type >> data[counter].produ_time >> data[counter].due_time >> data[counter].in_stack >> data[counter].sell_amount;
		counter++;
	}
	dat.close();
	counter--;
	if (counter == 0)
	{
		cout << "数据库中还没有添加过药品！";
		Sleep(1000);
		return 0;
	}
	sort(data, data + counter);
	cout << setw(35) << ' ' << "销售统计" << setw(35) << ' ' << setw(0) << endl;
	cout << "             名称                     库存数量          累计销量     \n";
	for (long long i = 0; i < counter; i++)
	{
		cout << setw(30) << data[i].name << setw(18) << data[i].in_stack << setw(18) << data[i].sell_amount << setw(0) << endl;
	}
	system("pause");
	return 0;
}

int sim()
{
	long long counter = 0, id, amount;
	int cmd;
	fstream dat;
	medi data[100];
	struct tm *p1 = new struct tm, *p2 = new struct tm;
	time_t now;
	dat.open("data.dat", ios::in);
	while (!dat.eof())
	{
		dat >> data[counter].name >> data[counter].type >> data[counter].produ_time >> data[counter].due_time >> data[counter].in_stack >> data[counter].sell_amount;
		counter++;
	}
	dat.close();
	counter--;
	if (counter == 0)
	{
		cout << "数据库中还没有添加过药品！";
		Sleep(1000);
		return 0;
	}
	while (1)
	{
		system("cls");
		cout << "*****模拟销售*****\n"
			<< "*1.销售          *\n"
			<< "*2.进货          *\n"
			<< "*3.结束          *\n"
			<< "*输入数字指令操作*\n";
		cin >> cmd;
		if (cmd == 1)
		{
			cout << "请输入销售的药品id";
			cin >> id;
			if (id < 0 || id >= counter)
			{
				cout << "无此id！";
				Sleep(1000);
				continue;
			}
			time(&now);
			if (data[id].due_time < now)
			{
				cout << "该药品已过期！";
				Sleep(1000);
				continue;
			}
			char pt[100], dt[100];
			localtime_s(p1, &data[id].produ_time);
			strftime(pt, 100, "%Y年%m月%d日", p1);//把时间格式化成想要的格式用于输出
			localtime_s(p2, &data[id].due_time);
			strftime(dt, 100, "%Y年%m月%d日", p2);
			cout << "**********************药品信息**********************\n"
				<< "*1.药品名称：" << setw(38) << data[id].name << setw(0) << "*\n"//setw(int x)把输出宽度设置为x
				<< "*2.药品类别：" << setw(38) << data[id].type << setw(0) << "*\n"
				<< "*3.生产日期：" << setw(38) << pt << setw(0) << "*\n"
				<< "*4.保质期至：" << setw(38) << dt << setw(0) << "*\n"
				<< "*5.库存数量：" << setw(38) << data[id].in_stack << setw(0) << "*\n"
				<< "*6.累计销量：" << setw(38) << data[id].sell_amount << setw(0) << "*\n"
				<< "********************请输入出售的数量****************\n";
			cin >> amount;
			if (amount <= 0)
			{
				cout << "无效的输入！";
				continue;
			}
			if (amount > data[id].in_stack)
			{
				cout << "库存不足！";
				Sleep(1000);
				continue;
			}
			else
			{
				data[id].in_stack -= amount;
				data[id].sell_amount += amount;
				cout<<"成功销售!";
				Sleep(1000);
			}
		}
		else if (cmd == 2)
		{
			cout << "请输入进货的药品id";
			cin >> id;
			if (id < 0 || id >= counter)
			{
				cout << "无此id！";
				Sleep(1000);
				continue;
			}
			time(&now);
			if (data[id].due_time < now)
			{
				cout << "该药品已过期！";
				Sleep(1000);
				continue;
			}
			char pt[100], dt[100];
			localtime_s(p1, &data[id].produ_time);
			strftime(pt, 100, "%Y年%m月%d日", p1);//把时间格式化成想要的格式用于输出
			localtime_s(p2, &data[id].due_time);
			strftime(dt, 100, "%Y年%m月%d日", p2);
			cout << "**********************药品信息**********************\n"
				<< "*1.药品名称：" << setw(38) << data[id].name << setw(0) << "*\n"//setw(int x)把输出宽度设置为x
				<< "*2.药品类别：" << setw(38) << data[id].type << setw(0) << "*\n"
				<< "*3.生产日期：" << setw(38) << pt << setw(0) << "*\n"
				<< "*4.保质期至：" << setw(38) << dt << setw(0) << "*\n"
				<< "*5.库存数量：" << setw(38) << data[id].in_stack << setw(0) << "*\n"
				<< "*6.累计销量：" << setw(38) << data[id].sell_amount << setw(0) << "*\n"
				<< "********************请输入进货的数量****************\n";
			cin >> amount;
			if (amount <= 0)
			{
				cout << "无效的输入！";
				continue;
			}
			data[id].in_stack += amount;
			cout << "进货成功！";
			Sleep(1000);
		}
		else if (cmd == 3)
		{
			dat.open("data.dat", ios::out | ios::trunc);
			for (long long i = 0; i < counter; i++)
			{
				dat << data[i].name << ' ' << data[i].type << ' ' << data[i].produ_time << ' ' << data[i].due_time << ' ' << data[i].in_stack << ' ' << data[i].sell_amount << endl;
			}
			dat.close();
			return 0;
		}
		else
		{
			cout << "指令错误！" << endl;
			Sleep(1000);
		}
	}
	return 0;
}

int Exit()
{
	cout << "感谢使用！";
	Sleep(1000);
	exit(0);
	return 0;
}

int file()
{
	fstream dat;
	char cmd;
	dat.open("data.dat", ios::in);
	if (dat.is_open())
	{
		cout << "成功检测到数据文件";
	}
	else
	{
		cout << "数据文件还未被创建，是否现在创建?(y/n)";
		cin >> cmd;
		if (cmd == 'y')
		{
			dat.close();
			dat.open("data.dat", ios::out);
			if (dat.is_open())
			{
				cout << "数据文件创建成功！";
				dat << "test_name" << ' ' << "test_class" << ' ' << 000 << ' ' << 000 << ' ' << 1 << ' ' << 1 << endl;
			}
			else
			{
				cout << "数据文件创建失败！";
				Sleep(1000);
				exit(0);
			}
		}
		else
		{
			cout << "无数据文件，系统即将退出！";
			Sleep(1000);
			exit(0);
		}
	}
	dat.close();
	Sleep(1000);
	return 0;
}

int menu()
{
	int command;
	cout << "*****欢迎使用药房销售系统*****\n"
		<< "*1.添加药品                  *\n"
		<< "*2.查询药品                  *\n"
		<< "*3.修改药品                  *\n"
		<< "*4.删除药品                  *\n"
		<< "*5.显示临期药品              *\n"
		<< "*6.生成销售统计              *\n"
		<< "*7.模拟销售                  *\n"
		<< "*8.系统退出                  *\n"
		<< "**请输入对应功能前的数字序号**\n";
	cin >> command;
	cin.clear();
	cin.ignore(1,'\n');
	return command;
}

