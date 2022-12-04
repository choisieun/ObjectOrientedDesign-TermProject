/*202102719 Choi Sieun*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

const int MAXBYTE = 75;
const int MAXLINE = 20;
vector<string> pages;
vector<string> page;

void initPages(vector<string>& pages);
void initPage(vector<string>& pages, vector<string>& page);

class Edit {
protected:
  vector<string>& pages;
  vector<string>& page;
public:
  Edit(vector<string>& pages, vector<string>& page) : pages(pages), page(page) {}
  bool editPage();
};

class EditForInsert : public Edit {
public:
  EditForInsert(vector<string>& pages, vector<string>& page) : Edit(pages, page) {}
  bool editPage(int line, int index, string word, string target) {
    int i = 0;
    int start_index = 0;
    while (i < pages.size()) {
      start_index = i;
      string str = "";
      int bytes = 0;
      for (int j = 0; j < MAXBYTE; j++) {
        if (i == pages.size()) {
          if (!target.compare(str)) {
            int len = 0;
            while (1) {
              string temp = pages[start_index++];
              for (i = 0; i < temp.length(); i++) {
                if (len == index) {
                  temp.insert(i, word);
                  pages[start_index - 1] = temp;
                  initPage(pages, page);
                  return true;
                }
                len++;
              }
            }
          } else {
            return false;
          }
        }

        string s = pages[i];
        bytes += s.length();
        if (bytes > MAXBYTE) {
          if (!target.compare(str)) {
            int len = 0;
            while (1) {
              string temp = pages[start_index++];
              for (i = 0; i < temp.length(); i++) {
               if (len == index) {
                temp.insert(i, word);
                pages[start_index - 1] = temp;
                initPage(pages, page);
                return true;
               }
               len++;
              }
            }
          } else {
            continue;
          }
        } else {
          str += s;
          i++;
        }
      }
    }
    return false;
  }
};

class EditForDelete : public Edit {
public:
  EditForDelete(vector<string>& pages, vector<string>& page) : Edit(pages, page) {}
  bool editPage(int line, int index, int delete_bytes, string target) {
    int i = 0;
    int start_index = 0;
    while (i < pages.size()) {
      start_index = i;
      string str = "";
      int bytes = 0;
      for (int j = 0; j < MAXBYTE; j++) {
        if (i == pages.size()) {
          if (!target.compare(str)) {
            int len = 0;
            while (1) {
              string temp = pages[start_index++];
              for (i = 0; i < temp.length(); i++) {
                if (len == index) {
                  temp.erase(i, delete_bytes);
                  pages[start_index - 1] = temp;
                  initPage(pages, page);
                  return true;
                }
                len++;
              }
            }
          } else {
            return false;
          }
        }

        string s = pages[i];
        bytes += s.length();
        if (bytes > MAXBYTE) {
          if (!target.compare(str)) {
            int len = 0;
            while (1) {
              string temp = pages[start_index++];
              for (i = 0; i < temp.length(); i++) {
               if (len == index) {
                temp.erase(i, delete_bytes);
                pages[start_index - 1] = temp;
                initPage(pages, page);
                return true;
               }
               len++;
              }
            }
          } else {
            continue;
          }
        } else {
          str += s;
          i++;
        }
      }
    }
    return false;
  }
};

class EditForSearch : public Edit {
public:
  EditForSearch(vector<string>& pages, vector<string>& page) : Edit(pages, page) {}
  bool editPage(string word) {
    int i;
    bool found = false;
    for (i = 0; i < pages.size(); i++) {
      if (pages[i] == word) {
        found = true;
        break;
      }
    }

    if (!found) {
      return false;
    }

    for (int j = 1; j <= MAXLINE; j++) {
      string line = "";
      int bytes = 0;
      for (int k = 0; k < MAXBYTE; k++) {
        if (i == pages.size()) {
          goto PRINTLINE;
        }
        word = pages[i];
        bytes += word.length();
        if (bytes > MAXBYTE) {
          goto PRINTLINE;
        } else {
          line += word;
          i++;
        }
      }
PRINTLINE:
      if (j < 10) { cout << " "; }
      cout << j << "| " << line << endl;
    }
    return true;
  }
};

class EditForChange : public Edit {
public:
  EditForChange(vector<string>& pages, vector<string>& page) : Edit(pages, page) {}
  bool editPage(string before, string after) {
    int i;
    for (i = 0; i < pages.size(); i++) {
      if (pages[i] == before) {
        pages[i] = after;
      }
    }
    initPage(pages, page);
    return true;
  }
};

class EditForSaveAndExit : public Edit {
public:
  EditForSaveAndExit(vector<string>& pages, vector<string>& page) : Edit(pages, page) {}
  bool editPage() {
    ofstream file("test.txt");
    for (string word : pages) {
      file << word;
    }
    file.close();
  }
};

void printMenu(bool first_page, bool last_page, bool can_not_search, bool wrong_input) {
  cout << "\n";
  for (int i = 0; i < 75; i++) { cout << '-'; }
  cout << "\nn:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료\n";
  for (int i = 0; i < 75; i++) { cout << '-'; }
  cout << "\n(콘솔메시지) ";
  if (first_page) { cout << "This is the first page!\n"; }
  else if (last_page) { cout << "This is the last page!\n"; }
  else if (can_not_search) { cout << "Can not search the word.\n"; }
  else if (wrong_input) { cout << "Wrong input! Try again.\n"; }
  else { cout << "\n"; }
  for (int i = 0; i < 75; i++) { cout << '-'; }
  cout << "\n입력: ";
}

void initPages(vector<string>& pages) {
  while (pages.size() != 0) {
    pages.pop_back();
  }
  ifstream file("test.txt");
  if (file.is_open()) {
    string word;
    while (file >> word) {
      pages.push_back(word);
      pages.push_back(" ");
    }
    pages.pop_back();
    file.close();
  }
}

void initPage(vector<string>& pages, vector<string>& page) {
  while (page.size() != 0) {
    page.pop_back();
  }
  int i = 0;
  while (i < pages.size()) {
    string line = "";
    int bytes = 0;
    for (int j = 0; j < MAXBYTE; j++) {
      if (i == pages.size()) {
        goto ADDLINE;
      }
      string word = pages[i];
      bytes += word.length();
      if (bytes > MAXBYTE) {
        goto ADDLINE;
      } else {
        line += word;
        i++;
      }
    }
ADDLINE:
    page.push_back(line);
  }
}

int main() {

  initPages(pages);
  initPage(pages, page);

  int i = 0;
  bool first_page = false;
  bool last_page = false;
  bool can_not_search = false;
  bool wrong_input = false;
  while (1) {
    if (page.size() - i < MAXLINE) {
      i -= MAXLINE - (page.size() - i);
    }

    for (int j = 1; j <= MAXLINE; j++) {
      if (j < 10) { cout << " "; }
      cout << j << "| " << page[i++] << endl;
    }
PASS:
    printMenu(first_page, last_page, can_not_search, wrong_input);
    first_page = false;
    last_page = false;
    can_not_search = false;
    wrong_input = false;

    string input;
    cin >> input;
    cout << endl;
    if (input == "n") {
      if (i == page.size()) {
        i -= MAXLINE;
        last_page = true;
        continue;
      } else {
        continue;
      }
    } 
    
    else if (input == "p") {
      i -= MAXLINE * 2;
      if (i < 0) {
        i = 0;
        first_page = true;
      }
    } 
    
    else if (input.substr(0, 1) == "i") {
      try {
        i -= MAXLINE;
        if (i < 0) { i = 0; }
        EditForInsert insert(pages, page);
        istringstream is(input.substr(2, input.length() - 3));
        string s;
        vector<string> v;
        while (getline(is, s, ',')) {
          v.push_back(s);
        }
        if (stoi(v[0]) < 1 || stoi(v[0]) > MAXLINE || stoi(v[1]) < 0 || stoi(v[1]) > MAXBYTE) {
          wrong_input = true;
          continue;
        } else if(!insert.editPage(stoi(v[0]), stoi(v[1]), v[2], page[stoi(v[0])-1])) {
          wrong_input = true;
          continue;
        } else {
          goto PASS;
        }
      } catch (exception e) {
        wrong_input = true;
        continue;
      }
    } 
    
    else if (input.substr(0, 1) == "d") {
      try {
        i -= MAXLINE;
        if (i < 0) { i = 0; }
        EditForDelete dele(pages, page);
        istringstream is(input.substr(2, input.length() - 3));
        string s;
        vector<string> v;
        while (getline(is, s, ',')) {
          v.push_back(s);
        }
        if (v.size() != 3) {
          wrong_input = true;
        } else if (stoi(v[0]) < 1 || stoi(v[0]) > MAXLINE || stoi(v[1]) < 0 || stoi(v[1]) > MAXBYTE) {
          wrong_input = true;
          continue;
        } else if(!dele.editPage(stoi(v[0]), stoi(v[1]), stoi(v[2]), page[stoi(v[0])-1])) {
          wrong_input = true;
          continue;
        } else {
          goto PASS;
        }
      } catch (exception e) {
        wrong_input = true;
        continue;
      }
    } 
    
    else if (input.substr(0, 1) == "c") {
      i -= MAXLINE;
      if (i < 0) { i = 0; }
      try {
        EditForChange change(pages, page);
        istringstream is(input.substr(2, input.length() - 3));
        string s;
        vector<string> v;
        while (getline(is, s, ',')) {
          v.push_back(s);
        }
        if (v.size() != 2) {
          wrong_input = true;
        } else if (!change.editPage(v[0], v[1])) {
          wrong_input = true;
          continue;
        } else {
          goto PASS;
        }
      } catch (exception e) {
        wrong_input = true;
        continue;
      }
    } 
    
    else if (input.substr(0, 1) == "s") {
      try {
        i -= MAXLINE;
        if (i < 0) { i = 0; }
        EditForSearch search(pages, page);
        string word = input.substr(2, input.length() - 3);
        if (!search.editPage(word)) {
          can_not_search = true;
          continue;
        } else {
          goto PASS;
        }
      } catch (exception e) {
        wrong_input = true;
        continue;
      }
    } 
    
    else if (input == "t") {
        EditForSaveAndExit saveAndExit(pages, page);
        saveAndExit.editPage();
        goto RET;
    }
    
    else {
      i -= MAXLINE;
      if (i < 0) { i = 0; }
      wrong_input = true;
      continue;
    }
  }

RET:
  return 0;
}
