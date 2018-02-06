#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
using namespace std;

#define True 1
#define False 0
#define Enter '\n'
#define TitikKoma ';'
#define NMaxMatriks 25

typedef struct {
	int PosisiAwalX;
	int PosisiAwalY;
	int PanjangKata;
	int Persilangan;
	char Menurun;
} ElmtCrossword;

typedef struct {
	int PanjangStringTulis;	
	int XTulis;
	int YTulis;
} Tulisan;

#define PosisiAwalX(E) (E).PosisiAwalX
#define PosisiAwalY(E) (E).PosisiAwalY
#define PanjangKata(E) (E).PanjangKata
#define IsMenurun(E) (E).Menurun
#define JumlahPersilangan(E) (E).Persilangan

#define PanjangStringTulis(T) (T).PanjangStringTulis
#define XTulis(T) (T).XTulis
#define YTulis(T) (T).YTulis

/* KAMUS GLOBAL */
char matriks[NMaxMatriks][NMaxMatriks];
vector<string> kata;
vector<ElmtCrossword> crossword;
vector <Tulisan> vtulis;
int n;

/* KAMUS UNTUK BACA FILE EKSTERNAL */
char CC;
FILE * pita;
string Word;

void ADV() {
	int retval;
	retval = fscanf(pita,"%c",&CC);
}

int CharToBil (char x) {
	return (int) (x - 48);
}

void readbilangan() {
	string bil;
	int i=0;
	ADV();
	while (CC != Enter) {
		bil[i] = CC;
		++i;
		ADV();
	}
	int j=0;
	n = 0;
	while (j < i) {
		n = n*10 + CharToBil(bil[j]);
		++j;
	}
}

void readmatrix() {
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			ADV();
			matriks[i][j] = CC;
		}
		ADV();
	}
}

void readkata() {
	int i = 0;
	ADV();
	while (!feof(pita)) {
		if (CC == TitikKoma) {
			kata.push_back(Word);
			Word.erase(Word.begin(), Word.end());
			i = 0;
		} else {
			Word.push_back(CC);
		}
		ADV();
	}
	kata.push_back(Word);
}

void readexternalfile() {
	string filename;
	cout << "Masukkan nama file: ";
	getline(cin,filename);
	pita = fopen(filename.c_str(),"r");
	readbilangan();
	readmatrix();
	readkata();
	fclose(pita);
}

void insertmendatar() {
	char FoundKosong = False;
	int i = 1;
	int j = 1;
	int panjang = 0;
	ElmtCrossword dummies;
	while (i <= n) {
		while (j <= n) {
			if (matriks[i][j] == '-') {
				if (!FoundKosong) {
					PosisiAwalX(dummies) = j;
					PosisiAwalY(dummies) = i;
					PanjangKata(dummies) = 1;
					IsMenurun(dummies) = False;
					JumlahPersilangan(dummies) = 0;
					FoundKosong = True;
				}
				else {
					++PanjangKata(dummies);
				}
			}
			else { // matriks == '#'
				if (FoundKosong) {
					if (PanjangKata(dummies) > 1) {
						crossword.push_back(dummies);
					}
					FoundKosong = False;	
				} 
				if (PanjangKata(dummies) > 0) {
					PanjangKata(dummies) = 0;
				}
			}
			++j;
		}
		if (FoundKosong) {
			if (PanjangKata(dummies) > 1) {
				crossword.push_back(dummies);
			}
			FoundKosong = False;	
		}
		if (PanjangKata(dummies) > 0) {
			PanjangKata(dummies) = 0;
		}
		++i;
		j=1;
	}
}

void insertmenurun() {
	char FoundKosong = False;
	int i = 1;
	int j = 1;
	int panjang = 0;
	ElmtCrossword dummies;
	while (j <= n) {
		while (i <= n) {
			if (matriks[i][j] == '-') {
				if (!FoundKosong) {
					PosisiAwalX(dummies) = j;
					PosisiAwalY(dummies) = i;
					PanjangKata(dummies) = 1;
					IsMenurun(dummies) = True;
					JumlahPersilangan(dummies) = 0;
					FoundKosong = True;
				}
				else {
					++PanjangKata(dummies);
				}
			}
			else { // matriks == '#'
				if (FoundKosong) {
					if (PanjangKata(dummies) > 1) {
						crossword.push_back(dummies);
					}
					FoundKosong = False;	
				} 
				if (PanjangKata(dummies) > 0) {
					PanjangKata(dummies) = 0;
				}
			}
			++i;
		}
		if (FoundKosong) {
			if (PanjangKata(dummies) > 1) {
				crossword.push_back(dummies);
			}
			FoundKosong = False;	
		}
		if (PanjangKata(dummies) > 0) {
			PanjangKata(dummies) = 0;
		}
		++j;
		i=1;
	}
}

void movematrixtopersilangan() {
	int matrikspersilangan[NMaxMatriks][NMaxMatriks] = {0};
	int k = 0;
	for (int i = 0; i < crossword.size(); ++i) {
		if (!IsMenurun(crossword[i])) {
			for (int j = PosisiAwalX(crossword[i]); j < (PosisiAwalX(crossword[i]) + PanjangKata(crossword[i])); ++j) {
				matrikspersilangan[PosisiAwalY(crossword[i])][j] = i+1;
			}
		} else {
			for (int j = PosisiAwalY(crossword[i]); j < (PosisiAwalY(crossword[i]) + PanjangKata(crossword[i])); ++j) {
				if (matrikspersilangan[j][PosisiAwalX(crossword[i])] != 0) {
					++JumlahPersilangan(crossword[(matrikspersilangan[j][PosisiAwalX(crossword[i])])-1]);
					++JumlahPersilangan(crossword[i]);
				}
			}
		}
	}
}

void movematrixtocrossword() {
	insertmendatar();
	insertmenurun();
	movematrixtopersilangan();
}

void ngesort () {
	sort( kata.begin(), kata.end(),
		[](const string& a, const string& b) {
        return a.length() > b.length() || a.length() == b.length() && a < b;
	});
	sort(crossword.begin(), crossword.end(), [](const ElmtCrossword& a, const ElmtCrossword& b) {
        return PanjangKata(a) > PanjangKata(b) || PanjangKata(a) == PanjangKata(b) && JumlahPersilangan(a) > JumlahPersilangan(b);
	});
}

void tulismatriks(int pointeridx, char *berhasil) {
	Tulisan dummies;
	int X = PosisiAwalX(crossword[pointeridx]);
	int Y = PosisiAwalY(crossword[pointeridx]);
	if (IsMenurun(crossword[pointeridx])) {
		while ((*berhasil) && (Y < PosisiAwalY(crossword[pointeridx]) + PanjangKata(crossword[pointeridx]))) {
			if (matriks[Y][X] == '-') {
				matriks[Y][X] = kata[pointeridx][(Y - PosisiAwalY(crossword[pointeridx]))];
				PanjangStringTulis(dummies) = PanjangKata(crossword[pointeridx]);
				XTulis(dummies) = X;
				YTulis(dummies) = Y;
				vtulis.push_back(dummies);
			} else {
				if (matriks[Y][X] != kata[pointeridx][(Y - PosisiAwalY(crossword[pointeridx]))]) {
					* berhasil = False;
				} else {
					//gangapa2in
				}
			}
			++Y;
		}
	} else {
		while ((*berhasil) && (X < PosisiAwalX(crossword[pointeridx]) + PanjangKata(crossword[pointeridx]))) {
			if (matriks[Y][X] == '-') {
				matriks[Y][X] = kata[pointeridx][(X - PosisiAwalX(crossword[pointeridx]))];
				PanjangStringTulis(dummies) = PanjangKata(crossword[pointeridx]);
				XTulis(dummies) = X;
				YTulis(dummies) = Y;
				vtulis.push_back(dummies);
			} else {
				if (matriks[Y][X] != kata[pointeridx][(X - PosisiAwalX(crossword[pointeridx]))]) {
					* berhasil = False;
				} else {
					//gangapa2in
				}
			}
			++X;
		}
	}
}

void menghapus (int panjang) {
	Tulisan dummies = vtulis.back();
	while (PanjangStringTulis(dummies) == panjang) {
		matriks[YTulis(dummies)][XTulis(dummies)] = '-';
		vtulis.pop_back();
		if (vtulis.empty()) {
			break;
		} else {
			dummies = vtulis.back();
		}
	}
}

char bektrek(int idxawal) {
	if (idxawal == kata.size()) {
		return True;
	} else {
		int idxakhir = idxawal;
		char Samapanjang = True;
		while((Samapanjang) && idxakhir < kata.size()) {
			if (kata[idxawal].length() != kata[idxakhir].length()) {
				Samapanjang = False;
			} else {
				++idxakhir;	
			}
		}
		--idxakhir;
		do {
			char berhasil = True;
			int pointeridx = idxawal;
			while (berhasil && (pointeridx <= idxakhir)) {
				tulismatriks(pointeridx, &berhasil);
				++pointeridx;
			}
			if (berhasil) {
				berhasil = bektrek(idxakhir+1);
			}
			if (!berhasil) {
				menghapus(PanjangKata(crossword[idxawal]));
			} else {
				return True;
			}
		} while (next_permutation(kata.begin()+idxawal, kata.begin()+idxakhir+1));
		return False;
	}
}

int main() {
	readexternalfile();
	cout << "\nMasukan telah dibaca. sedang memroses.";
	auto start = chrono::high_resolution_clock::now();
	movematrixtocrossword();
	ngesort();
	char sampah = bektrek(0);
	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double,milli> waktu = finish - start;
	cout << "\n\nwaktu = " << waktu.count() << " milidetik\n\n";
	for (int i = 1; i <= n; ++ i) {
		for (int j = 1; j <= n; ++ j) {
			cout << matriks[i][j];
		}
		cout << '\n';
	}
}