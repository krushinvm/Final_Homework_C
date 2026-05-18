#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <iomanip>
#include <cctype>
#include <windows.h>

using namespace std;

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

string toLower(const string& s) {
    string res = trim(s);
    for (char& c : res) c = tolower((unsigned char)c);
    return res;
}

class Country {
private:
    string name, nameLow;
    string capital, capitalLow;
    unsigned int population;
    float area;
    string industry, industryLow;
    string religion, religionLow;

public:
    Country() : population(0), area(0) {}
    Country(const string& n, const string& cap, unsigned int pop, float a,
        const string& ind, const string& rel)
        : name(n), capital(cap), population(pop), area(a), industry(ind), religion(rel) {
        nameLow = toLower(n);
        capitalLow = toLower(cap);
        industryLow = toLower(ind);
        religionLow = toLower(rel);
    }

    string getName()      const { return name; }
    string getCapital()   const { return capital; }
    unsigned int getPopulation() const { return population; }
    float getArea()       const { return area; }
    string getIndustry()  const { return industry; }
    string getReligion()  const { return religion; }

    string getNameLow()   const { return nameLow; }
    string getCapitalLow()const { return capitalLow; }
    string getIndustryLow()const { return industryLow; }
    string getReligionLow()const { return religionLow; }

    double getDensity() const { return (area > 0) ? population / (double)area : 0; }

    string toString() const {
        ostringstream oss;
        oss << left << setw(25) << name
            << setw(20) << capital
            << setw(15) << population
            << setw(12) << area
            << setw(20) << industry
            << setw(15) << religion;
        return oss.str();
    }

    string toFileString() const {
        return name + ";" + capital + ";" + to_string(population) + ";" + to_string(area) + ";" + industry + ";" + religion;
    }

    static Country fromFileString(const string& line) {
        stringstream ss(line);
        string name, cap, ind, rel, popStr, areaStr;
        getline(ss, name, ';');
        getline(ss, cap, ';');
        getline(ss, popStr, ';');
        getline(ss, areaStr, ';');
        getline(ss, ind, ';');
        getline(ss, rel, ';');
        name = trim(name);
        cap = trim(cap);
        ind = trim(ind);
        rel = trim(rel);
        unsigned int pop = (unsigned int)stoul(popStr);
        float a = stof(areaStr);
        return Country(name, cap, pop, a, ind, rel);
    }
};

class Database {
    vector<Country> data;
public:
    bool load(const string& file) {
        ifstream f(file);
        if (!f) { cerr << "Error: cannot open " << file << endl; return false; }
        data.clear();
        string line;
        while (getline(f, line)) if (!line.empty()) data.push_back(Country::fromFileString(line));
        f.close();
        cout << "Loaded " << data.size() << " records.\n";
        return true;
    }

    bool save(const string& file) const {
        ofstream f(file);
        if (!f) { cerr << "Error: cannot create " << file << endl; return false; }
        for (auto& c : data) f << c.toFileString() << endl;
        f.close();
        cout << "Saved " << data.size() << " records.\n";
        return true;
    }

    void display() const {
        if (data.empty()) { cout << "Database is empty.\n"; return; }
        cout << "\n" << left << setw(25) << "Name" << setw(20) << "Capital"
            << setw(15) << "Population" << setw(12) << "Area"
            << setw(20) << "Industry" << setw(15) << "Religion" << endl;
        cout << string(107, '-') << endl;
        for (auto& c : data) cout << c.toString() << endl;
        cout << endl;
    }

    void add() {
        string n, cap, ind, rel;
        unsigned int pop;
        float a;
        cout << "Enter name: "; cin.ignore(); getline(cin, n);
        cout << "Capital: "; getline(cin, cap);
        cout << "Population: "; cin >> pop;
        cout << "Area (sq km): "; cin >> a;
        cout << "Industry: "; cin.ignore(); getline(cin, ind);
        cout << "Religion: "; getline(cin, rel);
        data.emplace_back(n, cap, pop, a, ind, rel);
        cout << "Record added.\n";
    }

    void remove() {
        if (data.empty()) { cout << "No records.\n"; return; }
        display();
        cout << "Enter number to delete (1.." << data.size() << "): ";
        int idx; cin >> idx;
        if (idx < 1 || idx >(int)data.size()) { cout << "Invalid.\n"; return; }
        data.erase(data.begin() + idx - 1);
        cout << "Deleted.\n";
    }

    void sortByName() {
        sort(data.begin(), data.end(), [](const Country& a, const Country& b) {
            return a.getNameLow() < b.getNameLow();
            });
        cout << "Sorted by name.\n";
    }

    void search() const {
        if (data.empty()) { cout << "Database empty.\n"; return; }
        cout << "Enter country name: ";
        string q; cin.ignore(); getline(cin, q);
        string qlow = toLower(q);
        for (auto& c : data) {
            if (c.getNameLow() == qlow) {
                cout << "\nFound:\n";
                cout << left << setw(25) << "Name" << setw(20) << "Capital"
                    << setw(15) << "Population" << setw(12) << "Area"
                    << setw(20) << "Industry" << setw(15) << "Religion" << endl;
                cout << string(107, '-') << endl;
                cout << c.toString() << endl << endl;
                return;
            }
        }
        cout << "Not found.\n";
    }

    void filterPopulation() const {
        if (data.empty()) { cout << "Database empty.\n"; return; }
        unsigned int minP, maxP;
        cout << "Min population: "; cin >> minP;
        cout << "Max population: "; cin >> maxP;
        if (minP > maxP) swap(minP, maxP);
        cout << "\nCountries with population " << minP << ".." << maxP << ":\n";
        cout << left << setw(25) << "Name" << setw(20) << "Capital"
            << setw(15) << "Population" << setw(12) << "Area"
            << setw(20) << "Industry" << setw(15) << "Religion" << endl;
        cout << string(107, '-') << endl;
        bool any = false;
        for (auto& c : data) {
            if (c.getPopulation() >= minP && c.getPopulation() <= maxP) {
                cout << c.toString() << endl;
                any = true;
            }
        }
        if (!any) cout << "None.\n";
        cout << endl;
    }

    void groupByIndustry() const {
        if (data.empty()) { cout << "Database empty.\n"; return; }
        map<string, vector<Country>> groups;
        for (auto& c : data) groups[c.getIndustryLow()].push_back(c);
        for (auto& [key, vec] : groups) {
            sort(vec.begin(), vec.end(), [](const Country& a, const Country& b) {
                return a.getNameLow() < b.getNameLow();
                });
            cout << "\nIndustry: " << vec[0].getIndustry() << "\n";
            cout << string(vec[0].getIndustry().length() + 10, '-') << endl;
            for (auto& c : vec)
                cout << "  " << c.getName() << " (capital: " << c.getCapital()
                << ", population: " << c.getPopulation() << ", area: " << c.getArea() << ")\n";
        }
        cout << endl;
    }

    void exportHighDensity() {
        if (data.empty()) { cout << "Database empty.\n"; return; }
        double p;
        cout << "Density threshold (people/sq km): "; cin >> p;
        vector<Country> res;
        for (auto& c : data) if (c.getDensity() > p) res.push_back(c);
        if (res.empty()) { cout << "No countries.\n"; return; }
        string fname;
        cout << "Output file name: "; cin >> fname;
        ofstream f(fname);
        if (!f) { cerr << "Cannot create file.\n"; return; }
        for (auto& c : res) f << c.toFileString() << endl;
        f.close();
        cout << "Saved " << res.size() << " countries to " << fname << endl;
    }
};

void menu() {
    cout << "\n========== MENU ==========\n";
    cout << "1. Load DB from file\n";
    cout << "2. Save DB to file\n";
    cout << "3. Display all\n";
    cout << "4. Add record\n";
    cout << "5. Delete record\n";
    cout << "6. Sort by name\n";
    cout << "7. Search by name\n";
    cout << "8. Filter by population\n";
    cout << "9. Group by industry (task 1)\n";
    cout << "10. Export density > p (task 2)\n";
    cout << "0. Exit\n";
    cout << "Your choice: ";
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    Database db;
    int ch;
    do {
        menu();
        cin >> ch;
        switch (ch) {
        case 1: { string f; cout << "Filename: "; cin >> f; db.load(f); break; }
        case 2: { string f; cout << "Filename: "; cin >> f; db.save(f); break; }
        case 3: db.display(); break;
        case 4: db.add(); break;
        case 5: db.remove(); break;
        case 6: db.sortByName(); break;
        case 7: db.search(); break;
        case 8: db.filterPopulation(); break;
        case 9: db.groupByIndustry(); break;
        case 10: db.exportHighDensity(); break;
        case 0: cout << "Goodbye!\n"; break;
        default: cout << "Invalid choice.\n";
        }
    } while (ch != 0);
    return 0;
}
