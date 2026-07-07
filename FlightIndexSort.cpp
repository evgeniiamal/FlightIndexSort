#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

/*******************************************************************
*          Г Л О Б А Л Ь Н Ь Н Ы Е     К О Н С Т А Н Т Ы           *
********************************************************************/
const int N_MAX = 1000;   // максимальное количество строк


// Структура рейса ЛА
struct Flight
{
	string Time;          // Время входа в зону
	string Model;         // Марка ЛА
	string tailNum;       // Бортовой номер
	string flightNum;     // Номер рейса
};

// Структура строки индексного массива
struct Index_sort
{
	int ind;      // индекс - номер записи в исходном массиве
	string key;   // ключ для сортировки (время)
};

/******************************************************************
*              П Р О Т О Т И П Ы    Ф У Н К Ц И Й                 *
*******************************************************************/
// Эхо-печать исходных данных
bool Echo_Print(const char* FNAME);// имя файла

//Чтение данных из файла и входной контроль
bool Input_Flight(const char* FNAME, // имя файла
	Flight* pFl,       // указатель на структуру Flight
	int& Fl_counter);   // ссылка на счётчик количества рейсов


// Печать ошибок по их коду
void Print_Errors(const char* FNAME, // имя файла
	int err_code,      // код ошибки
	int str_ind);      // индекс строки с ошибкой

// Печать отсортированной структуры
void Sorted_Print(Flight* pFl,       // указатель на структуру Flight
	int Fl_counter,    // счётник количества рейсов
	Index_sort* pInd);  // указатель на структуру Index


/******************************************************************/
/*                    Н А Б О Р   Т Е С Т О В                     */
/******************************************************************/
//const char* FNAME = "no_file.txt";     // файл отсутствует 
//const char* FNAME = "wrong_test1.txt"; // недостаточно данных
//const char* FNAME = "wrong_test2.txt"; // лишние данные в записи
//const char* FNAME = "wrong_test3.txt"; // неверный формат времени
//const char* FNAME = "wrong_test4.txt"; // неверный формат марки ЛА
//const char* FNAME = "wrong_test5.txt"; // неверный формат бортового номера
//const char* FNAME = "wrong_test6.txt"; // неверный формат номера рейса
//const char* FNAME = "wrong_test7.txt"; // несколько типовых ошибок и пустая строка


//const char* FNAME = "test1.txt"; // избыточные пробелы и пустые строки
//const char* FNAME = "test2.txt"; // тестовый
//const char* FNAME = "test3.txt"; // одинаковые ключи
const char* FNAME = "test4.txt";


/******************************************************************
*              О С Н О В Н А Я      П Р О Г Р А М М А             *
******************************************************************/
int main()
{
	setlocale(LC_ALL, "RUSSIAN"); // подключение русского языка
	system("color F0");

	int N_flight = 0; // число записей самолетов
	bool Clear;     // наличие ошибок

	Flight Group[N_MAX];		// массив структур, хранящих данные о рейсах
	Index_sort IndexVec[N_MAX]; // индексный массив
	Echo_Print(FNAME);
	cout << "НАЙДЕННЫЕ ОШИБКИ\n\n";
	Clear = Input_Flight(FNAME, Group, N_flight); // Чтение данных из файла и входной контроль

	// Если обнаружены ошибки —  выход
	if (!Clear)
	{
		system("PAUSE");
		return 1;
	}// if
	else
	{
		cout << "Ошибок не найдено. Данные корректны. \n" << endl;
	}// else


	/*************************************************************
	*                   ИНДЕКСНАЯ СОРТИРОВКА                     *
	**************************************************************/

	// Заполнение индексного массива IndexVec
	for (int i = 0; i < N_flight; i++)
	{
		IndexVec[i].ind = i; // индекс - номер строки исходного массива
		IndexVec[i].key = Group[i].Time; // ключ — время входа в зону
	}// for i

	// Индексная сортировка методом выбора
	for (int i = 0; i < N_flight - 1; i++)
	{
		int min_ind = i; //индекс минимального элемента
		for (int j = i + 1; j < N_flight; j++)
		{
			if (IndexVec[j].key < IndexVec[min_ind].key) // если текущее значение меньше минимального
			{
				min_ind = j; // индекс минимума - текущий индекс
			}// if
		}// for j
		if (min_ind != i) // если найден новый минимум - перестановка
		{
			Index_sort temp = IndexVec[i];   // сохраняем текущее значение
			IndexVec[i] = IndexVec[min_ind]; // меняем местами текущее значение
			IndexVec[min_ind] = temp;        // и новый минимум
		}// if
	}// for i

	Sorted_Print(Group, N_flight, IndexVec); // Печать отсортированного списка рейсов

	system("PAUSE");
	return 0;
}//main


/*******************************************************************
*            Р Е А Л И З А Ц И Я    Ф У Н К Ц И Й                  *
*******************************************************************/

// Валидация времени
bool Valid_Time(string s)
{
	if (s.length() != 5 || s[2] != ':') // если длина не 5 символов с разделителем :
	{
		return false;
	}// if
	for (int i = 0; i < 5; i++) // цикл по элементам строки
	{
		if (i == 2) continue; // разделитель пропускаем
		if (s[i] < '0' || s[i] > '9') // если элемент не является цифрой
		{
			return false;
		}// if
	}// for i
	int h = (s[0] - '0') * 10 + (s[1] - '0'); // расчёт значения часов
	int m = (s[3] - '0') * 10 + (s[4] - '0'); // расчёт значения минут
	return (h <= 23 && m <= 59);  // корректность введенного времени
}//Valid_Time

// Валидация марки ЛА
bool Valid_Model(string s)
{
	if (s.empty()) return false; // строка пустая
	for (int i = 0; i < s.length(); i++)
	{
		char c = s[i]; // рассматриваем i-ый элемент строки

		bool Letter = ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= 'А' && c <= 'Я') || (c >= 'а' && c <= 'я')); // является буквой
		bool Num = (c >= '0' && c <= '9'); // является цифрой
		bool Dash = (c == '-'); // является тире

		if (!Letter && !Num && !Dash) // если не буква, не цифра и не тире
		{
			return false;
		}// if
	}// for i
	return true;
}//Valid_Model


// Валидация бортового номера
bool Valid_TailNum(string s)
{
	if (s.empty()) return false; // строка пустая
	if (s.length() != 6) // если длина не 6 символов
	{
		return false;
	}
	// проверка, что первый символ - буква
	if (!((s[0] >= 'A' && s[0] <= 'Z') || (s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'А' && s[0] <= 'Я') || (s[0] >= 'а' && s[0] <= 'я')))
	{
		return false;
	}
	// проверка, что второй символ - тире
	if (s[1] != '-')
	{
		return false;
	}
	// проверка, что далее 4 цифры номера
	for (int i = 2; i < s.length(); i++)
	{
		if (!(s[i] >= '0' && s[i] <= '9')) // если не цифра
		{
			return false;
		}// if
	}// for i
	return true;
}//Valid_TailNum


// Валидация номера рейса
bool Valid_FlightNum(string s)
{
	if (s.length() != 4) // если длина не 4 символа
	{
		return false;
	}// if
	for (int i = 0; i < s.length(); i++) // цикл по элементам строки
	{
		if (s[i] < '0' || s[i] > '9') // если элемент не является цифрой
		{
			return false;
		}// if
	}// for i
	return true;
}//Valid_FlightNum

/**********************************************************************************************************/

// Эхо-печать исходных данных
bool Echo_Print(const char* FNAME)
{
	ifstream fin(FNAME); // Открытие файла
	if (!fin)            // файл не был открыт
	{
		return false;
	}// if
	string line;
	int i = 1;
	cout << "\nИСХОДНЫЕ ДАННЫЕ\n" << endl;

	// Построчное чтение и вывод файла
	while (getline(fin, line))
	{
		cout << i << " " << line << endl;
		i++;
	}//while

	cout << endl << endl;

	fin.close();
}//Echo_Print


//Чтение данных из файла и входной контроль
bool Input_Flight(const char* FNAME, // имя файла
	Flight* pFl,       // указатель на структуру Flight
	int& Fl_counter)   // ссылка на счётчик количества рейсов
{
	int line_num = 1;     // номер строки
	Fl_counter = 0;       // счетчик чистых записей
	bool OK = true;       // флаг отсутствия ошибок в файле

	ifstream fin(FNAME); // Открытие файла
	if (!fin)            // файл не был открыт
	{
		Print_Errors(FNAME, 1, -1); // Код 1 - файл не найден
		return false;
	}// if

	string line;
	// Считываем файл строго построчно до самого конца
	while (getline(fin, line))
	{
		// Пропуск пустых строк
		if (line.empty())
		{
			line_num++; // увеличиваем счётчик строк
			continue;
		}// if

		stringstream ss(line); // считываем запись в строку
		Flight tempFl; // временная структура для анализа текущей записи
		bool Correct = true;   // флаг корректности текущей записи
		bool structOK = true;  // флаг корректности структуры (4 поля)

		// Считывание строго 4х полей из текущей строки
		if (!(ss >> tempFl.Time >> tempFl.Model >> tempFl.tailNum >> tempFl.flightNum))
		{
			structOK = false;
		}// if
		// Обработка ошибки структуры
		if (!structOK)
		{
			Print_Errors(FNAME, 2, line_num); // Код 2 - ошибка структуры
			Correct = false;
			OK = false;
			line_num++; // увеличиваем счётчик строк
			continue; // переход к следующей строке файла
		}// if

		// Проверка на наличие лишних данных в конце строки
		string extra;
		if (structOK && (ss >> extra))
		{
			Print_Errors(FNAME, 3, line_num); // Код 3 - лишние данные
			Correct = false;
			OK = false;
		}// if


		// Валидация корректности формата полей
		if (!Valid_Time(tempFl.Time))
		{
			Print_Errors(FNAME, 4, line_num); // Код 4 - ошибка времени
			Correct = false;
			OK = false;
		}// if
		if (!Valid_Model(tempFl.Model))
		{
			Print_Errors(FNAME, 5, line_num); // Код 5 - ошибка марки
			Correct = false;
			OK = false;
		}// if
		if (!Valid_TailNum(tempFl.tailNum))
		{
			Print_Errors(FNAME, 6, line_num); // Код 6 - ошибка бортового номера
			Correct = false;
			OK = false;
		}// if
		if (!Valid_FlightNum(tempFl.flightNum))
		{
			Print_Errors(FNAME, 7, line_num); // Код 7 - ошибка номера рейса
			Correct = false;
			OK = false;
		}// if

		// Если в строке не обнаружено никаких ошибок — сохраняем
		if (Correct)
		{
			pFl[Fl_counter] = tempFl;
			Fl_counter++;
		}// if

		line_num++; // переходим к счетчику следующей строки
	}// while 

	fin.close(); // Закрыть файл
	return OK;
}//Input_Flight

// Печать сообщений об ошибках
void Print_Errors(const char* FNAME,
	int err_code,
	int str_ind)
{
	switch (err_code)
	{
	case 1: cout << "Файл " << FNAME << " не был найден!" << endl; break;
	case 2: cout << "Строка " << str_ind << ": Неверный формат записи. Должно быть ровно 4 поля: время, марка, бортовой номер и номер рейса" << endl; break;
	case 3: cout << "Строка " << str_ind << ": Лишние данные в конце строки" << endl; break;
	case 4: cout << "Строка " << str_ind << ": Неверный формат времени. (Время должно быть в формате ЧЧ:ММ от 00:00 до 23:59)" << endl; break;
	case 5: cout << "Строка " << str_ind << ": Неверный формат марки ЛА. (Марка ЛА может включать только буквы, цифры и дефис)" << endl; break;
	case 6: cout << "Строка " << str_ind << ": Неверный бортовой номер. (Бортовой номер должеть быть вида: А-1234)" << endl; break;
	case 7: cout << "Строка " << str_ind << ": Неверный номер рейса. (Номер рейса может включать только цифры до 4 знаков)" << endl; break;

	}//switch
}//Print_Mess


// Печать отсортированной структуры
void Sorted_Print(Flight* pFl,       // указатель на структуру Flight
	int Fl_counter,    // счётник количества рейсов
	Index_sort* pInd)   // указатель на структуру Index
{
	cout << "\nОТСОРТИРОВАННЫЕ ПО ВРЕМЕНИ ДАННЫЕ О РЕЙСАХ\n\n";

	cout << "+------------+------------------+--------------+--------------+" << endl;
	cout << "|   Время    |     Марка ЛА     |  Бортовой №  |  Номер рейса |" << endl;
	cout << "+------------+------------------+--------------+--------------+" << endl;


	for (int i = 0; i < Fl_counter; i++)
	{
		int ind = pInd[i].ind; // извлекаем отсортированный индекс

		cout << "| " << left << setw(10) << pFl[ind].Time << " | "
			<< setw(16) << pFl[ind].Model << " | "
			<< setw(12) << pFl[ind].tailNum << " | "
			<< setw(12) << pFl[ind].flightNum << " |" << endl;
	}//for i

	// Нижняя граница
	cout << "+------------+------------------+--------------+--------------+" << endl;
	cout << endl;
}//Sorted_Print
