#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <windows.h>

using namespace std;

// Структура для одной страны
struct Country {
    string name;
    string capital;
    unsigned int population;
    float area;
    string industry;
    string religion;
};

vector<Country> baza;

// очистка пробелов и перевод в нижний регистр (для поиска и сортировки)
string toLower(string s) {
    int start = 0;
    while (start < s.size() && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r' || s[start] == '\n')) start++;
    int end = s.size() - 1;
    while (end >= 0 && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r' || s[end] == '\n')) end--;
    if (start > end) return "";
    string res = s.substr(start, end - start + 1);
    for (int i = 0; i < res.size(); i++) {
        res[i] = tolower((unsigned char)res[i]);
    }
    return res;
}

// Загрузка из файла
void zagruzit() {
    string filename;
    cout << "Имя файла: ";
    cin >> filename;
    ifstream fin(filename);
    if (!fin) {
        cout << "Не открыть файл!\n";
        return;
    }
    baza.clear();
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        if (line.size() >= 3 && (unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
            line = line.substr(3);
        }
        stringstream ss(line);
        string name, cap, ind, rel, popStr, areaStr;
        getline(ss, name, ';');
        getline(ss, cap, ';');
        getline(ss, popStr, ';');
        getline(ss, areaStr, ';');
        getline(ss, ind, ';');
        getline(ss, rel, ';');
        Country c;
        c.name = name;
        c.capital = cap;
        c.industry = ind;
        c.religion = rel;
        c.population = stoul(popStr);
        c.area = stof(areaStr);
        baza.push_back(c);
    }
    fin.close();
    cout << "Загружено " << baza.size() << " записей.\n";
}

// Сохранение в файл
void sohranit() {
    string filename;
    cout << "Имя файла: ";
    cin >> filename;
    ofstream fout(filename);
    if (!fout) {
        cout << "Не создать файл!\n";
        return;
    }
    for (int i = 0; i < baza.size(); i++) {
        fout << baza[i].name << ";" << baza[i].capital << ";" << baza[i].population << ";" << baza[i].area << ";" << baza[i].industry << ";" << baza[i].religion << endl;
    }
    fout.close();
    cout << "Сохранено " << baza.size() << " записей.\n";
}

// Показать всё
void pokazat() {
    if (baza.empty()) { cout << "База пуста.\n"; return; }
    cout << "\n";
    int w_name = 38;
    int w_capital = 24;
    int w_pop = 22;
    int w_area = 20;
    int w_ind = 55;
    int w_rel = 32;
    cout << left << setw(w_name) << "Название"
        << setw(w_capital) << "Столица"
        << setw(w_pop) << "Население"
        << setw(w_area) << "Площадь"
        << setw(w_ind) << "Промышленность"
        << setw(w_rel) << "Религия" << endl;
    cout << string(w_name + w_capital + w_pop + w_area + w_ind + w_rel, '-') << endl;
    for (int i = 0; i < baza.size(); i++) {
        cout << left << setw(w_name) << baza[i].name
            << setw(w_capital) << baza[i].capital
            << setw(w_pop) << baza[i].population
            << setw(w_area) << (int)baza[i].area
            << setw(w_ind) << baza[i].industry
            << setw(w_rel) << baza[i].religion << endl;
    }
    cout << endl;
}


// Добавлени записи
void dobavit() {
    Country c;
    cout << "Название: ";
    cin.ignore();
    getline(cin, c.name);
    cout << "Столица: ";
    getline(cin, c.capital);
    cout << "Население: ";
    cin >> c.population;
    cout << "Площадь: ";
    cin >> c.area;
    cout << "Промышленность: ";
    cin.ignore();
    getline(cin, c.industry);
    cout << "Религия: ";
    getline(cin, c.religion);
    baza.push_back(c);
    cout << "Добавлено.\n";
}

// Удалить по номеру
void udalit() {
    if (baza.empty()) {
        cout << "Нет записей.\n";
        return;
    }
    pokazat();
    cout << "Номер для удаления (1.." << baza.size() << "): ";
    int num;
    cin >> num;
    if (num < 1 || num > baza.size()) {
        cout << "Неверный номер.\n";
        return;
    }
    baza.erase(baza.begin() + num - 1);
    cout << "Удалено.\n";
}

// Сортировка по названию
void sortirovka() {
    for (int i = 0; i < baza.size(); i++) {
        for (int j = i + 1; j < baza.size(); j++) {
            string s1 = toLower(baza[i].name);
            string s2 = toLower(baza[j].name);
            if (s1 > s2) {
                swap(baza[i], baza[j]);
            }
        }
    }
    cout << "Отсортировано.\n";
}

// Поиск по названию
void poisk() {
    if (baza.empty()) {
        cout << "База пуста.\n";
        return;
    }
    cout << "Введите название: ";
    string query;
    cin.ignore();
    getline(cin, query);
    string qlow = toLower(query);
    for (int i = 0; i < baza.size(); i++) {
        if (toLower(baza[i].name) == qlow) {
            cout << "\nНайдено:\n";
            int w_name = 38;
            int w_capital = 24;
            int w_pop = 22;
            int w_area = 20;
            int w_ind = 55;
            int w_rel = 32;
            cout << left << setw(w_name) << "Название"
                << setw(w_capital) << "Столица"
                << setw(w_pop) << "Население"
                << setw(w_area) << "Площадь"
                << setw(w_ind) << "Промышленность"
                << setw(w_rel) << "Религия" << endl;
            cout << string(w_name + w_capital + w_pop + w_area + w_ind + w_rel, '-') << endl;
            cout << left << setw(w_name) << baza[i].name
                << setw(w_capital) << baza[i].capital
                << setw(w_pop) << baza[i].population
                << setw(w_area) << (int)baza[i].area
                << setw(w_ind) << baza[i].industry
                << setw(w_rel) << baza[i].religion << endl << endl;
            return;
        }
    }
    cout << "Не найдено.\n";
}

// Фильтр по населению
void filtr() {
    if (baza.empty()) {
        cout << "База пуста.\n";
        return;
    }
    unsigned int minp, maxp;
    cout << "Мин население: ";
    cin >> minp;
    cout << "Макс население: ";
    cin >> maxp;
    if (minp > maxp) swap(minp, maxp);
    cout << "\nСтраны с населением от " << minp << " до " << maxp << ":\n";
    cout << left << setw(25) << "Название" << setw(20) << "Столица" << setw(15) << "Население" << setw(12) << "Площадь" << setw(20) << "Промышленность" << setw(15) << "Религия" << endl;
    cout << string(107, '-') << endl;
    bool est = false;
    for (int i = 0; i < baza.size(); i++) {
        if (baza[i].population >= minp && baza[i].population <= maxp) {
            cout << left << setw(25) << baza[i].name << setw(20) << baza[i].capital << setw(15) << baza[i].population << setw(12) << baza[i].area << setw(20) << baza[i].industry << setw(15) << baza[i].religion << endl;
            est = true;
        }
    }
    if (!est) cout << "Нет.\n";
    cout << endl;
}

// Группировка по промышленности
void gruppirovka() {
    if (baza.empty()) {
        cout << "База пуста.\n";
        return;
    }
    // уникальные промышленности
    vector<string> industries;
    for (int i = 0; i < baza.size(); i++) {
        string ind = toLower(baza[i].industry);
        bool found = false;
        for (int j = 0; j < industries.size(); j++) {
            if (industries[j] == ind) { found = true; break; }
        }
        if (!found) industries.push_back(ind);
    }
    // Для каждой промышленности страны
    for (int g = 0; g < industries.size(); g++) {
        vector<Country> temp;
        for (int i = 0; i < baza.size(); i++) {
            if (toLower(baza[i].industry) == industries[g]) {
                temp.push_back(baza[i]);
            }
        }
        // сортировка внутри temp по названию
        for (int i = 0; i < temp.size(); i++) {
            for (int j = i + 1; j < temp.size(); j++) {
                if (toLower(temp[i].name) > toLower(temp[j].name)) {
                    swap(temp[i], temp[j]);
                }
            }
        }
        cout << "\nПромышленность: " << temp[0].industry << endl;
        cout << string(temp[0].industry.size() + 10, '-') << endl;
        for (int i = 0; i < temp.size(); i++) {
            cout << "  " << temp[i].name << " (столица: " << temp[i].capital << ", население: " << temp[i].population << ", площадь: " << temp[i].area << ")\n";
        }
    }
    cout << endl;
}

// Экспорт стран с плотностью > p
void exportPlotnost() {
    if (baza.empty()) {
        cout << "База пуста.\n";
        return;
    }
    double p;
    cout << "Порог плотности (чел/кв.км): ";
    cin >> p;
    vector<Country> result;
    for (int i = 0; i < baza.size(); i++) {
        double dens = (baza[i].area > 0) ? baza[i].population / (double)baza[i].area : 0;
        if (dens > p) result.push_back(baza[i]);
    }
    if (result.empty()) {
        cout << "Нет стран с такой плотностью.\n";
        return;
    }
    string fname;
    cout << "Имя файла для сохранения: ";
    cin >> fname;
    ofstream fout(fname);
    if (!fout) {
        cout << "Ошибка создания файла.\n";
        return;
    }
    for (int i = 0; i < result.size(); i++) {
        fout << result[i].name << ";" << result[i].capital << ";" << result[i].population << ";" << result[i].area << ";" << result[i].industry << ";" << result[i].religion << endl;
    }
    fout.close();
    cout << "Сохранено " << result.size() << " стран в файл " << fname << endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    int choice;
    do {
        cout << "\n========== МЕНЮ ==========\n";
        cout << "1. Загрузить БД из файла\n";
        cout << "2. Сохранить БД в файл\n";
        cout << "3. Просмотреть БД\n";
        cout << "4. Добавить запись\n";
        cout << "5. Удалить запись\n";
        cout << "6. Сортировка по названию\n";
        cout << "7. Поиск по названию\n";
        cout << "8. Фильтр по населению\n";
        cout << "9. Группировка по промышленности (задача 1)\n";
        cout << "10. Экспорт с плотностью > p (задача 2)\n";
        cout << "0. Выход\n";
        cout << "Ваш выбор: ";
        cin >> choice;
        switch (choice) {
        case 1: zagruzit(); break;
        case 2: sohranit(); break;
        case 3: pokazat(); break;
        case 4: dobavit(); break;
        case 5: udalit(); break;
        case 6: sortirovka(); break;
        case 7: poisk(); break;
        case 8: filtr(); break;
        case 9: gruppirovka(); break;
        case 10: exportPlotnost(); break;
        case 0: cout << "Пока!\n"; break;
        default: cout << "Неверный пункт\n";
        }
    } while (choice != 0);
    return 0;
}
