#pragma once
#include "TGra.h"
#include "TBazaPytan.h"
#include "TPytanie.h"
#include <msclr\marshal_cppstd.h>
#include <vector>
#include <string>

namespace Familiada {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Text;

	/// <summary>
	/// Podsumowanie informacji o MainWin
	/// </summary>
	public ref class MainWin : public System::Windows::Forms::Form
	{
	public:

		MainWin(void)
		{
			InitializeComponent();
			silnikGry = new TGra();

			pfc = gcnew PrivateFontCollection();

			Box = gcnew array<TextBox^, 2>(6, 2);
			try {
				// 2. £adujemy plik (upewnij siê, ¿e nazwa pliku jest identyczna!)
				pfc->AddFontFile("familiada.ttf");
				// 3. Sprawdzamy, czy siê za³adowa³a i przypisujemy do labela
				// pfc->Families[0] to nasza Familiada
				FamLbl->Font = gcnew System::Drawing::Font(pfc->Families[0], 96, FontStyle::Regular);
				GrajBtn->Font = gcnew System::Drawing::Font(pfc->Families[0], 48, FontStyle::Regular);
				DalejBtn->Font = gcnew System::Drawing::Font(pfc->Families[0], 48, FontStyle::Regular);
				NazwaDruzyny1TB->Font = gcnew System::Drawing::Font(pfc->Families[0], 32);
				NazwaDruzyny2TB->Font = gcnew System::Drawing::Font(pfc->Families[0], 32);

				LiczbaGraczyLbl->Font = gcnew System::Drawing::Font(pfc->Families[0], 20);
				// 4. Ustawienie wygl¹du (¿eby by³o jak w teleturnieju)
				FamLbl->ForeColor = Color::Yellow;
				GrajBtn->ForeColor = Color::Yellow;
				NazwaDruzyny1TB->ForeColor = Color::Yellow;
				NazwaDruzyny2TB->ForeColor = Color::Yellow;
				LiczbaGraczyLbl->ForeColor = Color::Yellow;

				NazwaDruzyny1TB->BackColor = Color::Black;
				NazwaDruzyny2TB->BackColor = Color::Black;

				NazwaDruzyny1TB->BorderStyle = System::Windows::Forms::BorderStyle::None; // Ukrywa bia³¹ ramkê
				NazwaDruzyny2TB->BorderStyle = System::Windows::Forms::BorderStyle::None;
				// Ukrywa bia³¹ ramkê

				NazwaDruzyny1TB->Text = "LEWA";
				NazwaDruzyny2TB->Text = "PRAWA";


				// --- KONFIGURACJA KAFELKÓW WYBORU (3, 4, 5, 6) ---
				array<Button^>^ przyciski = { btn3, btn4, btn5, btn6 };
				int startX = 374; // Pozycja X pierwszego przycisku (œrodek ekranu)
				int kafelekSize = 45;
				// Rozmiar boku kwadratu
				int odstep = 10;   // Przerwa miêdzy kafelkami

				for (int i = 0; i < przyciski->Length; i++) {
					// Rozmiar i pozycja
					przyciski[i]->Size = System::Drawing::Size(kafelekSize, kafelekSize);
					przyciski[i]->Location = System::Drawing::Point(startX + (i * (kafelekSize + odstep)), 70);

					// Wygl¹d "Familiada"
					przyciski[i]->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
					przyciski[i]->FlatAppearance->BorderSize = 2;
					przyciski[i]->FlatAppearance->BorderColor = Color::Yellow;
					przyciski[i]->BackColor = Color::Black;
					przyciski[i]->ForeColor = Color::Yellow;

					// Czcionka (mniejsza ni¿ w labelach, ¿eby siê zmieœci³a)
					if (pfc->Families->Length > 0) {
						przyciski[i]->Font = gcnew System::Drawing::Font(pfc->Families[0], 18);
					}

					// Podpiêcie wspólnego zdarzenia (jeœli jeszcze nie podpiête w Designerze)
					przyciski[i]->Click -= gcnew System::EventHandler(this, &MainWin::WybierzLiczbeGraczy_Click);
					// Na wszelki wypadek usuñ stare
					przyciski[i]->Click += gcnew System::EventHandler(this, &MainWin::WybierzLiczbeGraczy_Click);
				}


				this->BackColor = Color::Black;
			}
			catch (Exception^ ex) {
				// Jeœli pliku nie bêdzie, program siê nie wywali, tylko poka¿e b³¹d
				MessageBox::Show("Nie uda³o siê za³adowaæ czcionki: " + ex->Message);
			}

			txtOdliczanie->Multiline = true;         // Pozwala na wiele linii
			txtOdliczanie->WordWrap = true;          // Zawijanie tekstu
			txtOdliczanie->TextAlign = HorizontalAlignment::Center; // Wyœrodkowanie
			txtOdliczanie->Height = 400;
			// Zwiêksz wysokoœæ, ¿eby tekst siê nie uci¹³
			txtOdliczanie->Width = 960;
			// Wymuœ szerokoœæ okna (960)
			txtOdliczanie->Location = System::Drawing::Point(0, 100);
			// Przesuñ do krawêdzi, by wycentrowaæ idealnie
			// ==========================================
			// POPRAWKA STYLÓW (KOLORY I CZCIONKI)
			// ==========================================
			txtOdliczanie->BackColor = Color::Black;
			txtOdliczanie->ForeColor = Color::Yellow;
			TrescPytaniaLBL->BackColor = Color::Transparent;
			TrescPytaniaLBL->ForeColor = Color::Yellow;

			if (pfc->Families->Length > 0) {
				txtOdliczanie->Font = gcnew System::Drawing::Font(pfc->Families[0], 48, FontStyle::Regular);
				TrescPytaniaLBL->Font = gcnew System::Drawing::Font(pfc->Families[0], 36, FontStyle::Regular);
			}
			// Œcie¿ka do katalogu z zasobami (plik czcionki i obrazki)
			String^ assetsPath = System::IO::Path::Combine(Application::StartupPath, "Assets");

			// £adujemy obrazki z katalogu Assets (bez awarii, jeœli brakuje plików)
			String^ leftOff = System::IO::Path::Combine(assetsPath, "buzzer-dark-red.png");
			String^ leftOn = System::IO::Path::Combine(assetsPath, "buzzer-light-red.png");
			String^ rightOff = System::IO::Path::Combine(assetsPath, "buzzer-dark-green.png");
			String^ rightOn = System::IO::Path::Combine(assetsPath, "buzzer-light-green.png");
			try {
				if (System::IO::File::Exists(leftOff)) imgLewoWygaszone = System::Drawing::Image::FromFile(leftOff);
				if (System::IO::File::Exists(leftOn)) imgLewoSwiecace = System::Drawing::Image::FromFile(leftOn);
				if (System::IO::File::Exists(rightOff)) imgPrawoWygaszone = System::Drawing::Image::FromFile(rightOff);
				if (System::IO::File::Exists(rightOn)) imgPrawoSwiecace = System::Drawing::Image::FromFile(rightOn);
			}
			catch (Exception^) {
				// Nie przerywamy dzia³ania aplikacji, pozostawiamy obrazki niezainicjalizowane
			}
			//MessageBox::Show("Konstruktor dzia³a");
			InicjalizujPanelOdpowiedzi();
			InicjalizujBledy();
			WskaznikDruzynyStrzalkaPrawa = gcnew Label();
			WskaznikDruzynyStrzalkaLewa = gcnew Label();

			WskaznikDruzynyStrzalkaPrawa->AutoSize = false;
			WskaznikDruzynyStrzalkaPrawa->Size = System::Drawing::Size(200, 60);
			WskaznikDruzynyStrzalkaLewa->AutoSize = false;
			WskaznikDruzynyStrzalkaLewa->Size = System::Drawing::Size(200, 60);

			WskaznikDruzynyStrzalkaPrawa->Location = Point(780, 510); // dó³ ekranu
			WskaznikDruzynyStrzalkaPrawa->TextAlign = ContentAlignment::MiddleCenter;
			WskaznikDruzynyStrzalkaLewa->Location = Point(15, 510);
			// dó³ ekranu
			WskaznikDruzynyStrzalkaLewa->TextAlign = ContentAlignment::MiddleCenter;

			WskaznikDruzynyStrzalkaPrawa->BackColor = Color::Black;
			WskaznikDruzynyStrzalkaPrawa->ForeColor = Color::Yellow;
			WskaznikDruzynyStrzalkaLewa->BackColor = Color::Black;
			WskaznikDruzynyStrzalkaLewa->ForeColor = Color::Yellow;
			WskaznikDruzynyStrzalkaPrawa->Font = gcnew System::Drawing::Font(pfc->Families[0], 36);
			WskaznikDruzynyStrzalkaLewa->Font = gcnew System::Drawing::Font(pfc->Families[0], 36);

			WskaznikDruzynyStrzalkaPrawa->Visible = false;
			WskaznikDruzynyStrzalkaLewa->Visible = false;

			PanelOdpowiedzi->Controls->Add(WskaznikDruzynyStrzalkaPrawa);
			PanelOdpowiedzi->Controls->Add(WskaznikDruzynyStrzalkaLewa);
			WskaznikDruzynyStrzalkaPrawa->Text = "->";

			WskaznikDruzynyStrzalkaLewa->Text = "<-";

			InicjalizujPanelFinal();




			finalQuestions = gcnew cli::array<String^>(5);

			finalQuestions[0] = "Wymien owoc";
			finalQuestions[1] = "Wymien zwierzê";
			finalQuestions[2] = "Wymien kolor";
			finalQuestions[3] = "Wymien sport";
			finalQuestions[4] = "Wymien napój";

			
		}

	protected:
		/// <summary>
		/// Wyczyœæ wszystkie u¿ywane zasoby.
		/// </summary>
		~MainWin()
		{
			if (components)
			{
				delete components;
			}
			delete silnikGry;
		}
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Button^ GrajBtn;
	private: System::Windows::Forms::Label^ FamLbl;
	private: System::Windows::Forms::Panel^ PanelStart;
	private: System::Windows::Forms::Panel^ PanelGracze;
	private: System::Windows::Forms::Panel^ PanelPrawaDruzyna;
	private: System::Windows::Forms::Panel^ PanelLewaDruzyna;
	private: System::Windows::Forms::ComboBox^ PlayersCountCB;
	private: System::Drawing::Text::PrivateFontCollection^ pfc;
	private: System::Windows::Forms::Label^ DruzynaPrawaLbl;
	private: System::Windows::Forms::Label^ DruzynaLewaLbl;
	private: System::Windows::Forms::TextBox^ NazwaDruzyny2TB;
	private: System::Windows::Forms::TextBox^ NazwaDruzyny1TB;
	private: System::Windows::Forms::Button^ btn6;

	private: System::Windows::Forms::Button^ btn5;

	private: System::Windows::Forms::Button^ btn4;

	private: System::Windows::Forms::Button^ btn3;
	private: System::Windows::Forms::Label^ LiczbaGraczyLbl;

		   //private: System::Windows::Forms::TextBox^ 
	private: array<TextBox^, 2>^ Box;
	private: System::Windows::Forms::Button^ DalejBtn;
	private: System::Windows::Forms::Panel^ PanelPytanie;
	private: System::Windows::Forms::Label^ TrescPytaniaLBL;
	private: System::Windows::Forms::Label^ TrescPytaniaLBL2;
	private: System::Windows::Forms::Panel^ PanelCzek;

	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::TextBox^ txtOdliczanie;
	private: System::Windows::Forms::PictureBox^ PrawaDruzBuzzPB;

	private: System::Windows::Forms::PictureBox^ LewaDruzBuzzPB;

	private: System::ComponentModel::IContainer^ components;
	private: System::Drawing::Image^ imgLewoWygaszone;
	private: System::Drawing::Image^ imgLewoSwiecace;

	private: System::Drawing::Image^ imgPrawoWygaszone;
	private: System::Drawing::Image^ imgPrawoSwiecace;
	private: System::Windows::Forms::PictureBox^ PrawaDruzzBuzzONpb;
	private: System::Windows::Forms::PictureBox^ LewaDruzzBuzzONpb;

	private: System::Windows::Forms::Panel^ PanelOdpowiedzi;

	Label^ SumaPunkty;

	private:
		int aktualnaSumaPunktow = 0; // Ta zmienna bêdzie pamiêtaæ wynik





		   //private: cli::array<System::Windows::Forms::TextBox^>^ odpHaslo;
	private: System::Windows::Forms::Label^ WskaznikDruzyny;
	private:
		cli::array<System::Windows::Forms::Label^>^ bledyLewo;
		cli::array<System::Windows::Forms::Label^>^ bledyPrawo;

	private: System::Windows::Forms::Panel^ PanelFinal;

	private: System::Windows::Forms::Label^ WskaznikDruzynyStrzalkaPrawa;
	private: System::Windows::Forms::Label^ WskaznikDruzynyStrzalkaLewa;

	private: System::Windows::Forms::Label^ FinalPlayerLbl;
	private: System::Windows::Forms::Label^ FinalQuestionLbl;
	private: System::Windows::Forms::Label^ FinalTimerLbl;

	private: System::Windows::Forms::TextBox^ FinalAnswerTB;


	private:
		cli::array<String^>^ finalQuestions;

	private:
		int finalQuestionIndex = 0;

	private: System::Windows::Forms::TextBox^ OdpowiedzTB;

	private:
		cli::array<Label^>^ odpNr;
		cli::array<Label^>^ odpHaslo;
		cli::array<Label^>^ odpPunkty;

	private: Label^ PunktyDruzyny1LBL;
	private: Label^ PunktyDruzyny2LBL;

	private: TGra* silnikGry;










	protected:

	protected:

	protected:

	private:
		/// <summary>
		/// Wymagana zmienna projektanta.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metoda wymagana do obs³ugi projektanta — nie nale¿y modyfikowaæ
		/// jej zawartoœci w edytorze kodu.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainWin::typeid));
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->GrajBtn = (gcnew System::Windows::Forms::Button());
			this->FamLbl = (gcnew System::Windows::Forms::Label());
			this->PanelStart = (gcnew System::Windows::Forms::Panel());
			this->PanelGracze = (gcnew System::Windows::Forms::Panel());
			this->DalejBtn = (gcnew System::Windows::Forms::Button());
			this->LiczbaGraczyLbl = (gcnew System::Windows::Forms::Label());
			this->btn6 = (gcnew System::Windows::Forms::Button());
			this->btn5 = (gcnew System::Windows::Forms::Button());
			this->btn4 = (gcnew System::Windows::Forms::Button());
			this->btn3 = (gcnew System::Windows::Forms::Button());
			this->NazwaDruzyny2TB = (gcnew System::Windows::Forms::TextBox());
			this->NazwaDruzyny1TB = (gcnew System::Windows::Forms::TextBox());
			this->DruzynaPrawaLbl = (gcnew System::Windows::Forms::Label());
			this->DruzynaLewaLbl = (gcnew System::Windows::Forms::Label());
			this->PanelPrawaDruzyna = (gcnew System::Windows::Forms::Panel());
			this->PanelLewaDruzyna = (gcnew System::Windows::Forms::Panel());
			this->PlayersCountCB = (gcnew System::Windows::Forms::ComboBox());
			this->PanelPytanie = (gcnew System::Windows::Forms::Panel());
			this->PrawaDruzzBuzzONpb = (gcnew System::Windows::Forms::PictureBox());
			this->LewaDruzzBuzzONpb = (gcnew System::Windows::Forms::PictureBox());
			this->PrawaDruzBuzzPB = (gcnew System::Windows::Forms::PictureBox());
			this->LewaDruzBuzzPB = (gcnew System::Windows::Forms::PictureBox());
			this->TrescPytaniaLBL = (gcnew System::Windows::Forms::Label());
			this->PanelCzek = (gcnew System::Windows::Forms::Panel());
			this->txtOdliczanie = (gcnew System::Windows::Forms::TextBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->PanelOdpowiedzi = (gcnew System::Windows::Forms::Panel());
			this->PanelFinal = (gcnew System::Windows::Forms::Panel());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->PanelStart->SuspendLayout();
			this->PanelGracze->SuspendLayout();
			this->PanelPytanie->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PrawaDruzzBuzzONpb))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->LewaDruzzBuzzONpb))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PrawaDruzBuzzPB))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->LewaDruzBuzzPB))->BeginInit();
			this->PanelCzek->SuspendLayout();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::Black;
			this->pictureBox1->Location = System::Drawing::Point(18, 18);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(1440, 900);
			this->pictureBox1->TabIndex = 2;
			this->pictureBox1->TabStop = false;
			// 
			// GrajBtn
			// 
			this->GrajBtn->BackColor = System::Drawing::Color::Black;
			this->GrajBtn->Location = System::Drawing::Point(472, 709);
			this->GrajBtn->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->GrajBtn->Name = L"GrajBtn";
			this->GrajBtn->Size = System::Drawing::Size(494, 125);
			this->GrajBtn->TabIndex = 4;
			this->GrajBtn->Text = L"GRAJ";
			this->GrajBtn->UseVisualStyleBackColor = false;
			this->GrajBtn->Click += gcnew System::EventHandler(this, &MainWin::GrajBtn_Click);
			this->GrajBtn->MouseLeave += gcnew System::EventHandler(this, &MainWin::GrajBtn_MouseLeave);
			this->GrajBtn->MouseHover += gcnew System::EventHandler(this, &MainWin::GrajBtn_MouseHover);
			// 
			// FamLbl
			// 
			this->FamLbl->AutoSize = true;
			this->FamLbl->ForeColor = System::Drawing::Color::Yellow;
			this->FamLbl->Location = System::Drawing::Point(78, 192);
			this->FamLbl->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->FamLbl->Name = L"FamLbl";
			this->FamLbl->Size = System::Drawing::Size(96, 20);
			this->FamLbl->TabIndex = 3;
			this->FamLbl->Text = L"FAMILIADA";
			// 
			// PanelStart
			// 
			this->PanelStart->Controls->Add(this->FamLbl);
			this->PanelStart->Controls->Add(this->GrajBtn);
			this->PanelStart->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PanelStart->Location = System::Drawing::Point(0, 0);
			this->PanelStart->Name = L"PanelStart";
			this->PanelStart->Size = System::Drawing::Size(1440, 900);
			this->PanelStart->TabIndex = 5;
			this->PanelStart->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainWin::PanelStart_Paint);
			// 
			// PanelGracze
			// 
			this->PanelGracze->Controls->Add(this->DalejBtn);
			this->PanelGracze->Controls->Add(this->LiczbaGraczyLbl);
			this->PanelGracze->Controls->Add(this->btn6);
			this->PanelGracze->Controls->Add(this->btn5);
			this->PanelGracze->Controls->Add(this->btn4);
			this->PanelGracze->Controls->Add(this->btn3);
			this->PanelGracze->Controls->Add(this->NazwaDruzyny2TB);
			this->PanelGracze->Controls->Add(this->NazwaDruzyny1TB);
			this->PanelGracze->Controls->Add(this->DruzynaPrawaLbl);
			this->PanelGracze->Controls->Add(this->DruzynaLewaLbl);
			this->PanelGracze->Controls->Add(this->PanelPrawaDruzyna);
			this->PanelGracze->Controls->Add(this->PanelLewaDruzyna);
			this->PanelGracze->Controls->Add(this->PlayersCountCB);
			this->PanelGracze->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PanelGracze->Location = System::Drawing::Point(0, 0);
			this->PanelGracze->Name = L"PanelGracze";
			this->PanelGracze->Size = System::Drawing::Size(1440, 900);
			this->PanelGracze->TabIndex = 6;
			// 
			// DalejBtn
			// 
			this->DalejBtn->BackColor = System::Drawing::Color::Black;
			this->DalejBtn->Location = System::Drawing::Point(472, 709);
			this->DalejBtn->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->DalejBtn->Name = L"DalejBtn";
			this->DalejBtn->Size = System::Drawing::Size(494, 125);
			this->DalejBtn->TabIndex = 4;
			this->DalejBtn->Text = L"DALEJ";
			this->DalejBtn->UseVisualStyleBackColor = false;
			this->DalejBtn->Visible = false;
			this->DalejBtn->Click += gcnew System::EventHandler(this, &MainWin::DalejBtn_Click);
			this->DalejBtn->MouseLeave += gcnew System::EventHandler(this, &MainWin::DalejBtn_MouseLeave);
			this->DalejBtn->MouseHover += gcnew System::EventHandler(this, &MainWin::DalejBtn_MouseHover);
			// 
			// LiczbaGraczyLbl
			// 
			this->LiczbaGraczyLbl->AutoSize = true;
			this->LiczbaGraczyLbl->Location = System::Drawing::Point(540, 46);
			this->LiczbaGraczyLbl->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->LiczbaGraczyLbl->Name = L"LiczbaGraczyLbl";
			this->LiczbaGraczyLbl->Size = System::Drawing::Size(138, 20);
			this->LiczbaGraczyLbl->TabIndex = 14;
			this->LiczbaGraczyLbl->Text = L"LICZBA GRACZY";
			// 
			// btn6
			// 
			this->btn6->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btn6->Location = System::Drawing::Point(802, 122);
			this->btn6->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->btn6->Name = L"btn6";
			this->btn6->Size = System::Drawing::Size(38, 35);
			this->btn6->TabIndex = 12;
			this->btn6->Text = L"6";
			this->btn6->UseVisualStyleBackColor = true;
			// 
			// btn5
			// 
			this->btn5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btn5->Location = System::Drawing::Point(722, 126);
			this->btn5->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->btn5->Name = L"btn5";
			this->btn5->Size = System::Drawing::Size(40, 35);
			this->btn5->TabIndex = 11;
			this->btn5->Text = L"5";
			this->btn5->UseVisualStyleBackColor = true;
			// 
			// btn4
			// 
			this->btn4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btn4->Location = System::Drawing::Point(660, 125);
			this->btn4->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->btn4->Name = L"btn4";
			this->btn4->Size = System::Drawing::Size(52, 35);
			this->btn4->TabIndex = 10;
			this->btn4->Text = L"4";
			this->btn4->UseVisualStyleBackColor = true;
			// 
			// btn3
			// 
			this->btn3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btn3->Location = System::Drawing::Point(570, 126);
			this->btn3->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->btn3->Name = L"btn3";
			this->btn3->Size = System::Drawing::Size(81, 35);
			this->btn3->TabIndex = 9;
			this->btn3->Text = L"3";
			this->btn3->UseVisualStyleBackColor = true;
			this->btn3->Click += gcnew System::EventHandler(this, &MainWin::WybierzLiczbeGraczy_Click);
			// 
			// NazwaDruzyny2TB
			// 
			this->NazwaDruzyny2TB->Location = System::Drawing::Point(1042, 123);
			this->NazwaDruzyny2TB->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->NazwaDruzyny2TB->Name = L"NazwaDruzyny2TB";
			this->NazwaDruzyny2TB->Size = System::Drawing::Size(373, 26);
			this->NazwaDruzyny2TB->TabIndex = 6;
			this->NazwaDruzyny2TB->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->NazwaDruzyny2TB->Click += gcnew System::EventHandler(this, &MainWin::NazwaDruzyny2TB_Click);
			this->NazwaDruzyny2TB->Leave += gcnew System::EventHandler(this, &MainWin::NazwaDruzyny2TB_Leave);
			// 
			// NazwaDruzyny1TB
			// 
			this->NazwaDruzyny1TB->Location = System::Drawing::Point(38, 123);
			this->NazwaDruzyny1TB->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->NazwaDruzyny1TB->Name = L"NazwaDruzyny1TB";
			this->NazwaDruzyny1TB->Size = System::Drawing::Size(373, 26);
			this->NazwaDruzyny1TB->TabIndex = 5;
			this->NazwaDruzyny1TB->Click += gcnew System::EventHandler(this, &MainWin::NazwaDruzyny1TB_Click);
			this->NazwaDruzyny1TB->Leave += gcnew System::EventHandler(this, &MainWin::NazwaDruzyny1TB_Leave);
			// 
			// DruzynaPrawaLbl
			// 
			this->DruzynaPrawaLbl->Location = System::Drawing::Point(0, 0);
			this->DruzynaPrawaLbl->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->DruzynaPrawaLbl->Name = L"DruzynaPrawaLbl";
			this->DruzynaPrawaLbl->Size = System::Drawing::Size(150, 35);
			this->DruzynaPrawaLbl->TabIndex = 7;
			// 
			// DruzynaLewaLbl
			// 
			this->DruzynaLewaLbl->Location = System::Drawing::Point(0, 0);
			this->DruzynaLewaLbl->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->DruzynaLewaLbl->Name = L"DruzynaLewaLbl";
			this->DruzynaLewaLbl->Size = System::Drawing::Size(150, 35);
			this->DruzynaLewaLbl->TabIndex = 8;
			// 
			// PanelPrawaDruzyna
			// 
			this->PanelPrawaDruzyna->AutoSize = true;
			this->PanelPrawaDruzyna->Location = System::Drawing::Point(880, 214);
			this->PanelPrawaDruzyna->Name = L"PanelPrawaDruzyna";
			this->PanelPrawaDruzyna->Size = System::Drawing::Size(543, 669);
			this->PanelPrawaDruzyna->TabIndex = 2;
			// 
			// PanelLewaDruzyna
			// 
			this->PanelLewaDruzyna->AutoSize = true;
			this->PanelLewaDruzyna->Location = System::Drawing::Point(18, 214);
			this->PanelLewaDruzyna->Name = L"PanelLewaDruzyna";
			this->PanelLewaDruzyna->Size = System::Drawing::Size(543, 669);
			this->PanelLewaDruzyna->TabIndex = 1;
			// 
			// PlayersCountCB
			// 
			this->PlayersCountCB->Location = System::Drawing::Point(0, 0);
			this->PlayersCountCB->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->PlayersCountCB->Name = L"PlayersCountCB";
			this->PlayersCountCB->Size = System::Drawing::Size(180, 28);
			this->PlayersCountCB->TabIndex = 13;
			this->PlayersCountCB->Visible = false;
			// 
			// PanelPytanie
			// 
			this->PanelPytanie->Controls->Add(this->PrawaDruzzBuzzONpb);
			this->PanelPytanie->Controls->Add(this->LewaDruzzBuzzONpb);
			this->PanelPytanie->Controls->Add(this->PrawaDruzBuzzPB);
			this->PanelPytanie->Controls->Add(this->LewaDruzBuzzPB);
			this->PanelPytanie->Controls->Add(this->TrescPytaniaLBL);
			this->PanelPytanie->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PanelPytanie->Location = System::Drawing::Point(0, 0);
			this->PanelPytanie->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->PanelPytanie->Name = L"PanelPytanie";
			this->PanelPytanie->Size = System::Drawing::Size(1440, 900);
			this->PanelPytanie->TabIndex = 7;
			// 
			// PrawaDruzzBuzzONpb
			// 
			this->PrawaDruzzBuzzONpb->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PrawaDruzzBuzzONpb.Image")));
			this->PrawaDruzzBuzzONpb->Location = System::Drawing::Point(1130, 635);
			this->PrawaDruzzBuzzONpb->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->PrawaDruzzBuzzONpb->Name = L"PrawaDruzzBuzzONpb";
			this->PrawaDruzzBuzzONpb->Size = System::Drawing::Size(212, 198);
			this->PrawaDruzzBuzzONpb->TabIndex = 6;
			this->PrawaDruzzBuzzONpb->TabStop = false;
			this->PrawaDruzzBuzzONpb->Visible = false;
			// 
			// LewaDruzzBuzzONpb
			// 
			this->LewaDruzzBuzzONpb->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"LewaDruzzBuzzONpb.Image")));
			this->LewaDruzzBuzzONpb->Location = System::Drawing::Point(88, 635);
			this->LewaDruzzBuzzONpb->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->LewaDruzzBuzzONpb->Name = L"LewaDruzzBuzzONpb";
			this->LewaDruzzBuzzONpb->Size = System::Drawing::Size(212, 198);
			this->LewaDruzzBuzzONpb->TabIndex = 5;
			this->LewaDruzzBuzzONpb->TabStop = false;
			this->LewaDruzzBuzzONpb->Visible = false;
			// 
			// PrawaDruzBuzzPB
			// 
			this->PrawaDruzBuzzPB->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PrawaDruzBuzzPB.Image")));
			this->PrawaDruzBuzzPB->Location = System::Drawing::Point(1130, 635);
			this->PrawaDruzBuzzPB->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->PrawaDruzBuzzPB->Name = L"PrawaDruzBuzzPB";
			this->PrawaDruzBuzzPB->Size = System::Drawing::Size(210, 198);
			this->PrawaDruzBuzzPB->TabIndex = 4;
			this->PrawaDruzBuzzPB->TabStop = false;
			// 
			// LewaDruzBuzzPB
			// 
			this->LewaDruzBuzzPB->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"LewaDruzBuzzPB.Image")));
			this->LewaDruzBuzzPB->Location = System::Drawing::Point(88, 635);
			this->LewaDruzBuzzPB->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->LewaDruzBuzzPB->Name = L"LewaDruzBuzzPB";
			this->LewaDruzBuzzPB->Size = System::Drawing::Size(212, 198);
			this->LewaDruzBuzzPB->TabIndex = 3;
			this->LewaDruzBuzzPB->TabStop = false;
			// 
			// TrescPytaniaLBL
			// 
			this->TrescPytaniaLBL->AutoSize = true;
			TrescPytaniaLBL->Location = System::Drawing::Point((1440 - TrescPytaniaLBL->Width) / 2, 150);
			this->TrescPytaniaLBL->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->TrescPytaniaLBL->Name = L"TrescPytaniaLBL";
			this->TrescPytaniaLBL->TabIndex = 2;
			this->TrescPytaniaLBL->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			TrescPytaniaLBL->Left = (this->ClientSize.Width - TrescPytaniaLBL->Width) / 2;
			// 
			// PanelCzek
			// 
			this->PanelCzek->Controls->Add(this->txtOdliczanie);
			this->PanelCzek->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PanelCzek->Location = System::Drawing::Point(0, 0);
			this->PanelCzek->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->PanelCzek->Name = L"PanelCzek";
			this->PanelCzek->Size = System::Drawing::Size(1440, 900);
			this->PanelCzek->TabIndex = 8;
			// 
			// txtOdliczanie
			// 
			this->txtOdliczanie->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->txtOdliczanie->Location = System::Drawing::Point(150, 123);
			this->txtOdliczanie->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->txtOdliczanie->Name = L"txtOdliczanie";
			this->txtOdliczanie->ReadOnly = true;
			this->txtOdliczanie->Size = System::Drawing::Size(150, 19);
			this->txtOdliczanie->TabIndex = 0;
			// 
			// timer1
			// 
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &MainWin::timer1_Tick);
			// 
			// PanelOdpowiedzi
			// 
			this->PanelOdpowiedzi->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PanelOdpowiedzi->Location = System::Drawing::Point(0, 0);
			this->PanelOdpowiedzi->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->PanelOdpowiedzi->Name = L"PanelOdpowiedzi";
			this->PanelOdpowiedzi->Size = System::Drawing::Size(1440, 900);
			this->PanelOdpowiedzi->TabIndex = 9;
			// 
			// PanelFinal
			// 
			this->PanelFinal->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PanelFinal->Location = System::Drawing::Point(0, 0);
			this->PanelFinal->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->PanelFinal->Name = L"PanelFinal";
			this->PanelFinal->Size = System::Drawing::Size(1440, 900);
			this->PanelFinal->TabIndex = 10;
			// 
			// MainWin
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(1440, 900);
			this->Controls->Add(this->PanelStart);
			this->Controls->Add(this->PanelFinal);
			this->Controls->Add(this->PanelCzek);
			this->Controls->Add(this->PanelOdpowiedzi);
			this->Controls->Add(this->PanelPytanie);
			this->Controls->Add(this->PanelGracze);
			this->Controls->Add(this->pictureBox1);
			this->KeyPreview = true;
			this->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->MaximizeBox = false;
			this->Name = L"MainWin";
			this->Text = L"Familiada";
			this->Load += gcnew System::EventHandler(this, &MainWin::MainWin_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainWin::MainWin_KeyDown);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->PanelStart->ResumeLayout(false);
			this->PanelStart->PerformLayout();
			this->PanelGracze->ResumeLayout(false);
			this->PanelGracze->PerformLayout();
			this->PanelPytanie->ResumeLayout(false);
			this->PanelPytanie->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PrawaDruzzBuzzONpb))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->LewaDruzzBuzzONpb))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PrawaDruzBuzzPB))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->LewaDruzBuzzPB))->EndInit();
			this->PanelCzek->ResumeLayout(false);
			this->PanelCzek->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void MainWin_Load(System::Object^ sender, System::EventArgs^ e) {}

	private: System::Void OnTextBoxClick(System::Object^ sender, System::EventArgs^ e) {

		TextBox^ tb = (TextBox^)(sender);
		System::Drawing::Point p = (System::Drawing::Point)(tb->Tag);

		if (tb->Text == "GRACZ " + (p.X + 1)) tb->Text = "";
	}

	private: System::Void OnTextBoxLeave(System::Object^ sender, System::EventArgs^ e) {

		TextBox^ tb = (TextBox^)(sender);
		System::Drawing::Point p = (System::Drawing::Point)(tb->Tag);
		if (tb->Text == "") tb->Text = "GRACZ " + (p.X + 1);
	}

	private: System::Void GrajBtn_MouseHover(System::Object^ sender, System::EventArgs^ e) {

		GrajBtn->ForeColor = Color::Black;
		GrajBtn->BackColor = System::Drawing::Color::Yellow;
	}
	private: System::Void GrajBtn_MouseLeave(System::Object^ sender, System::EventArgs^ e) {
		GrajBtn->ForeColor = Color::Yellow;
		GrajBtn->BackColor = System::Drawing::Color::Black;
	}
	private: System::Void GrajBtn_Click(System::Object^ sender, System::EventArgs^ e) {
		//GrajBtn->Visible = false;
		//FamLbl->Visible = false;

		PokazEkran(PanelGracze);
		//PanelGracze->Visible = true;
	}
	private: System::Void PanelStart_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {}

	private: System::Void PokazEkran(Panel^ ekran) {

		PanelStart->Visible = false;
		//PanelGracze->Visible = false;
		//PanelPytanie->Visible = false;
		//PanelOdpowiedz->Visible = false;
		//PanelFinal->Visible = false;

		// Pokazujemy tylko to, co chcemy
		ekran->Visible = true;
		ekran->BringToFront(); // Wa¿ne, ¿eby wskoczy³ na wierzch

		if (ekran == PanelPytanie) {
			silnikGry->UstawNasluchiwanie(true);
			silnikGry->OdblokujBuzzery(); // Resetujemy stan przy wejœciu
		}
		else {
			silnikGry->UstawNasluchiwanie(false);
		}
	}
		   //private: System::Void PlayersCountCB_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		   //	// 1. Pobieramy wybran¹ liczbê (np. 3, 4, 5 lub 6)
		   //	int count = System::Convert::ToInt32(PlayersCountCB->SelectedItem);
		   //
		   //	// 2. Czyœcimy stare pola, ¿eby siê nie nak³ada³y
		   //	PanelLewaDruzyna->Controls->Clear();
		   //	PanelPrawaDruzyna->Controls->Clear();
		   //
		   //	for (int i = 0; i < count; i++) {
		   //		// --- DRU¯YNA 1 (Lewa) ---
		   //		Box[i, 0] = gcnew TextBox();
		   //		Box[i, 0]->Text = "GRACZ " + (i + 1);
		   //		Box[i, 0]->Width = 250;
		   //		Box[i, 0]->BackColor = Color::Black;
		   //		Box[i, 0]->ForeColor = Color::Yellow;
		   //		Box[i, 0]->Font = gcnew System::Drawing::Font(pfc->Families[0], 20); // Twoja czcionka!
		   //		Box[i, 0]->Location = Point(10, i * 60);
		   // Odstêp 60px w pionie
		   //		PanelLewaDruzyna->Controls->Add(Box[i, 0]);
		   //
		   //		// --- DRU¯YNA 2 (Prawa - Odbicie) ---
		   //		TextBox^ tb2 = gcnew TextBox();
		   //		tb2->Text = "GRACZ " + (i + 1);
		   //		tb2->Width = 250;
		   //		tb2->BackColor = Color::Black;
		   //		tb2->ForeColor = Color::Yellow;
		   //		tb2->Font = gcnew System::Drawing::Font(pfc->Families[0], 20);
		   //		// Odbicie lustrzane: ustawiamy tekst do prawej
		   //		tb2->TextAlign = HorizontalAlignment::Right;
		   //		tb2->Location = Point(PanelPrawaDruzyna->Width - tb2->Width - 10, i * 60);
		   //		PanelPrawaDruzyna->Controls->Add(tb2);
		   //	}
		   //}

	private: System::Void NazwaDruzyny1TB_Click(System::Object^ sender, System::EventArgs^ e) {
		if (NazwaDruzyny1TB->Text == "LEWA") NazwaDruzyny1TB->Text = "";
	}
	private: System::Void NazwaDruzyny2TB_Click(System::Object^ sender, System::EventArgs^ e) {
		if (NazwaDruzyny2TB->Text == "PRAWA") NazwaDruzyny2TB->Text = "";
	}
	private: System::Void WybierzLiczbeGraczy_Click(System::Object^ sender, System::EventArgs^ e) {
		// 1. Rozpoznajemy który przycisk klikniêto
		Button^ klikniety = safe_cast<Button^>(sender);
		int count = System::Convert::ToInt32(klikniety->Text);
		// 2. Wizualne zaznaczenie wyboru
		array<Button^>^ przyciski = { btn3, btn4, btn5, btn6 };
		for each (Button ^ b in przyciski) {
			b->BackColor = Color::Black;
			b->ForeColor = Color::Yellow;
		}
		klikniety->BackColor = Color::Yellow;
		klikniety->ForeColor = Color::Black;
		// 3. Generowanie pól
		PanelLewaDruzyna->Controls->Clear();
		PanelPrawaDruzyna->Controls->Clear();

		for (int i = 0; i < count; i++) {
			// --- DRU¯YNA 1 (Lewa) ---
			Box[i, 0] = gcnew TextBox();
			Box[i, 0]->Text = "GRACZ " + (i + 1);
			Box[i, 0]->Width = 250;
			Box[i, 0]->BackColor = Color::Black;
			Box[i, 0]->ForeColor = Color::Yellow;
			Box[i, 0]->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			Box[i, 0]->Font = gcnew System::Drawing::Font(pfc->Families[0], 20); // Twoja czcionka!
			Box[i, 0]->Location = Point(10, i * 60); // Odstêp 60px w pionie
			Box[i, 0]->Click += gcnew EventHandler(this, &MainWin::OnTextBoxClick);
			Box[i, 0]->Leave += gcnew EventHandler(this, &MainWin::OnTextBoxLeave);
			PanelLewaDruzyna->Controls->Add(Box[i, 0]);

			Box[i, 0]->Tag = System::Drawing::Point(i, 0);

			// Prawa strona
			Box[i, 1] = gcnew TextBox();
			Box[i, 1]->Text = "GRACZ " + (i + 1);
			Box[i, 1]->Width = 250;
			Box[i, 1]->BackColor = Color::Black;
			Box[i, 1]->ForeColor = Color::Yellow;
			Box[i, 1]->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			Box[i, 1]->Font = gcnew System::Drawing::Font(pfc->Families[0], 20);
			Box[i, 1]->TextAlign = HorizontalAlignment::Right;
			Box[i, 1]->Location = Point(PanelPrawaDruzyna->Width - Box[i, 1]->Width - 10, i * 60);
			Box[i, 1]->Click += gcnew EventHandler(this, &MainWin::OnTextBoxClick);
			Box[i, 1]->Leave += gcnew EventHandler(this, &MainWin::OnTextBoxLeave);
			PanelPrawaDruzyna->Controls->Add(Box[i, 1]);

			//System::Drawing::Point(i, 1);
			Box[i, 1]->Tag = System::Drawing::Point(i, 1);

			DalejBtn->Visible = true;
		}

		//DalejBtn->BackColor = System::Drawing::Color::Black;

	}
	private: System::Void NazwaDruzyny1TB_Leave(System::Object^ sender, System::EventArgs^ e) {
		if (NazwaDruzyny1TB->Text == "") NazwaDruzyny1TB->Text = "LEWA";
	}
	private: System::Void NazwaDruzyny2TB_Leave(System::Object^ sender, System::EventArgs^ e) {
		if (NazwaDruzyny2TB->Text == "") NazwaDruzyny2TB->Text = "PRAWA";
	}
	private: System::Void DalejBtn_MouseHover(System::Object^ sender, System::EventArgs^ e) {

		DalejBtn->ForeColor = Color::Black;
		DalejBtn->BackColor = System::Drawing::Color::Yellow;
	}

	private: System::Void DalejBtn_MouseLeave(System::Object^ sender, System::EventArgs^ e) {
		DalejBtn->ForeColor = Color::Yellow;
		DalejBtn->BackColor = System::Drawing::Color::Black;
	}


	private:
		int czasPozostaly = 6;
	private: System::Void DalejBtn_Click(System::Object^ sender, System::EventArgs^ e) {
		// ==========================================
	// 1. POBIERANIE NAZW DRU¯YN
	// ==========================================

	// U¿ywamy Twoich dok³adnych nazw TextBoxów!
		System::String^ textDruzynaL = NazwaDruzyny1TB->Text;
		std::string nazwaLewa = msclr::interop::marshal_as<std::string>(textDruzynaL);

		System::String^ textDruzynaP = NazwaDruzyny2TB->Text;
		std::string nazwaPrawa = msclr::interop::marshal_as<std::string>(textDruzynaP);

		// Zabezpieczenie: jeœli gracz zostawi³ puste pole albo domyœlny napis, nadajemy nazwê awaryjn¹
		if (nazwaLewa == "" || nazwaLewa == "LEWA") {
			nazwaLewa = "Dru¿yna Lewa";
		}
		if (nazwaPrawa == "" || nazwaPrawa == "PRAWA") {
			nazwaPrawa = "Dru¿yna Prawa";
		}

		// Odpalamy silnik z pewnymi nazwami!
		silnikGry->inicjalizuj(nazwaLewa, nazwaPrawa);
		silnikGry->getDruzynaLewa()->ResetujBledy();
		silnikGry->getDruzynaPrawa()->ResetujBledy();
		// ==========================================
		// [NOWE] £ADOWANIE I LOSOWANIE PYTANIA Z BAZY JSON
		// ==========================================
		silnikGry->wczytajPytaniaZPliku("familiada_pytania.json");
		silnikGry->losujPytanie();

		// Pobieramy wylosowane pytanie z silnika i konwertujemy na string z Windows Forms
		TPytanie wylosowane = silnikGry->getAktualnePytanie();
		System::String^ trescStr = gcnew System::String(wylosowane.getTresc().c_str());

		// Zastêpujemy "AAAAA..." prawdziwym pytaniem na panelu buzzerów i odpowiedzi
		TrescPytaniaLBL->Text = trescStr;
		if (TrescPytaniaLBL2 != nullptr) {
			TrescPytaniaLBL2->Text = trescStr;
		}

		// ==========================================
		// 2. POBIERANIE GRACZY Z TABLICY BOX
		// ==========================================

		int count = Box->GetLength(0);

		for (int i = 0; i < count; i++) {
			// -- Lewa strona --
			if (Box[i, 0] != nullptr && !String::IsNullOrWhiteSpace(Box[i, 0]->Text)) {
				System::String^ textLewy = Box[i, 0]->Text;
				std::string imieLewe = msclr::interop::marshal_as<std::string>(textLewy);
				silnikGry->getDruzynaLewa()->dodajGracza(imieLewe);
			}

			// -- Prawa strona --
			if (Box[i, 1] != nullptr && !String::IsNullOrWhiteSpace(Box[i, 1]->Text)) {
				System::String^ textPrawy = Box[i, 1]->Text;
				std::string imiePrawe = msclr::interop::marshal_as<std::string>(textPrawy);
				silnikGry->getDruzynaPrawa()->dodajGracza(imiePrawe);
			}
		}

	//	// ==========================================
	//// --- KOD TESTOWY (Do weryfikacji danych) ---
	//// ==========================================
	//	System::String^ raport = "=== RAPORT ZAPISANYCH DANYCH ===\n\n";

	//	// --- SPRAWDZANIE LEWEJ DRU¯YNY ---
	//	std::string nazwaL = silnikGry->getDruzynaLewa()->getNazwa();
	//	std::vector<std::string> graczeL = silnikGry->getDruzynaLewa()->getGracze();

	//	raport += "DRU¯YNA: " + gcnew System::String(nazwaL.c_str()) + "\n";
	//	raport += "Zapisano graczy: " + graczeL.size() + "\n";
	//	for (size_t i = 0; i < graczeL.size(); i++) {
	//		// Znak += dokleja kolejne linijki tekstu do naszego raportu
	//		raport += " -> " + gcnew System::String(graczeL[i].c_str()) + "\n";
	//	}

	//	raport += "\n----------------------------------\n\n";

	//	// --- SPRAWDZANIE PRAWEJ DRU¯YNY ---
	//	std::string nazwaP = silnikGry->getDruzynaPrawa()->getNazwa();
	//	std::vector<std::string> graczeP = silnikGry->getDruzynaPrawa()->getGracze();

	//	raport += "DRU¯YNA: " + gcnew System::String(nazwaP.c_str()) + "\n";
	//	raport += "Zapisano graczy: " + graczeP.size() + "\n";
	//	for (size_t i = 0; i < graczeP.size(); i++) {
	//		raport += " -> " + gcnew System::String(graczeP[i].c_str()) + "\n";
	//	}

	//	// Wyœwietlamy nasz gotowy raport!
	//	MessageBox::Show(raport, "Sukces - Test weryfikacji");

		// ==========================================
		// ==========================================
		// 3. PRZEJŒCIE DO EKRANU GRY
		// ==========================================

		//GrajBtn->Visible = false;
		//FamLbl->Visible = false;

		PokazEkran(PanelCzek);
		//PanelGracze->Visible = true;

		czasPozostaly = 6;

		timer1->Start();
		aktualnaSumaPunktow = 0;
		this->SumaPunkty->Text = "....";
	}


	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
		czasPozostaly--;


		txtOdliczanie->Text = "ROZGRYWKA\r\n\r\nROZPOCZNIE SIE ZA\r\n\r\n " + czasPozostaly.ToString();

		if (czasPozostaly <= 0)
		{
			timer1->Stop();
			PokazEkran(PanelPytanie);
		}
	}




	private: System::Void ResetBuzzerow() {
		LewaDruzBuzzPB->Image = imgLewoWygaszone;
		PrawaDruzBuzzPB->Image = imgPrawoWygaszone;
		silnikGry->OdblokujBuzzery();
	}


	private: System::Void MainWin_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
	{

		if (e->KeyCode == Keys::F)
		{
			finalQuestionIndex = 0;

			PokazPytanieFinalowe();

			FinalPlayerLbl->Text = "GRACZ 1";
			FinalTimerLbl->Text = "20";
			FinalAnswerTB->Clear();

			PokazEkran(PanelFinal);
			return;
		}
		// B³êdy dzia³aj¹ zawsze
		/*if (e->KeyCode == Keys::X)
		{
			DodajBlad();
			return;
		}*/

		// Dalej logika buzzerów
		if (!silnikGry->CzyNasluchujemy()) return;
		if (silnikGry->CzyBuzzerZablokowany()) return;

		if (e->KeyCode == Keys::Q)
		{
			if (silnikGry->WcisnijBuzzer(Druzyna::Lewa)) {
				LewaDruzBuzzPB->Visible = false;
				LewaDruzzBuzzONpb->Visible = true;

				PokazEkran(PanelOdpowiedzi);
				UstawAktywnaDruzyne(Druzyna::Lewa);
			}
		}
		else if (e->KeyCode == Keys::P)
		{
			if (silnikGry->WcisnijBuzzer(Druzyna::Prawa)) {
				PrawaDruzBuzzPB->Visible = false;
				PrawaDruzzBuzzONpb->Visible = true;

				PokazEkran(PanelOdpowiedzi);
				UstawAktywnaDruzyne(Druzyna::Prawa);
			}
		}
	}

	private: System::Void InicjalizujPanelOdpowiedzi()
	{

		// PUNKTY DRU¯YNY 1
		PunktyDruzyny1LBL = gcnew Label();
		PunktyDruzyny1LBL->Text = "999";
		PunktyDruzyny1LBL->Size = Drawing::Size(100, 60);
		PunktyDruzyny1LBL->Location = Point(NazwaDruzyny1TB->Left, NazwaDruzyny1TB->Bottom + 5);

		PunktyDruzyny1LBL->BackColor = Color::Transparent;
		PunktyDruzyny1LBL->ForeColor = Color::Yellow;
		PunktyDruzyny1LBL->TextAlign = ContentAlignment::MiddleCenter;
		PunktyDruzyny1LBL->Font =
			gcnew Drawing::Font(pfc->Families[0], 20);

		PanelOdpowiedzi->Controls->Add(PunktyDruzyny1LBL);
		// PUNKTY DRU¯YNY 2
		PunktyDruzyny2LBL = gcnew Label();
		PunktyDruzyny2LBL->Text = "999";
		PunktyDruzyny2LBL->Size = Drawing::Size(100, 60);
		PunktyDruzyny2LBL->Location = Point(
			this->ClientSize.Width - PunktyDruzyny2LBL->Width - 20,
			NazwaDruzyny2TB->Bottom + 5);

		PunktyDruzyny2LBL->BackColor = Color::Transparent;
		PunktyDruzyny2LBL->ForeColor = Color::Yellow;
		PunktyDruzyny2LBL->TextAlign = ContentAlignment::MiddleCenter;
		PunktyDruzyny2LBL->Font =
			gcnew Drawing::Font(pfc->Families[0], 20);

		PanelOdpowiedzi->Controls->Add(PunktyDruzyny2LBL);

		// NAZWA DRUZYNY 1 
		Label^ NazwaDruzyny1LBL = gcnew Label();

		NazwaDruzyny1LBL->Text = NazwaDruzyny1TB->Text;
		NazwaDruzyny1LBL->Font = gcnew System::Drawing::Font(pfc->Families[0], 30, FontStyle::Regular);
		NazwaDruzyny1LBL->ForeColor = Color::Yellow;

		NazwaDruzyny1LBL->Size = Drawing::Size(NazwaDruzyny1TB->Size);
		NazwaDruzyny1LBL->Location = Point(
			PunktyDruzyny1LBL->Left,
			PunktyDruzyny1LBL->Top - 45);

		NazwaDruzyny1LBL->BackColor = Color::Black;
		NazwaDruzyny1LBL->ForeColor = Color::Yellow;

		NazwaDruzyny1LBL->Font =
			gcnew Drawing::Font(pfc->Families[0], 20);


		PanelOdpowiedzi->Controls->Add(NazwaDruzyny1LBL);

		// NAZWA DRUZYNY 2

		Label^ NazwaDruzyny2LBL = gcnew Label();

		NazwaDruzyny2LBL->Text = NazwaDruzyny2TB->Text;
		NazwaDruzyny2LBL->Font = gcnew System::Drawing::Font(pfc->Families[0], 30, FontStyle::Regular);
		NazwaDruzyny2LBL->ForeColor = Color::Yellow;

		NazwaDruzyny2LBL->Size = Drawing::Size(NazwaDruzyny1TB->Size);
		NazwaDruzyny2LBL->Location = Point(695,
			PunktyDruzyny2LBL->Top - 45);

		NazwaDruzyny2LBL->BackColor = Color::Black;
		NazwaDruzyny2LBL->ForeColor = Color::Yellow;

		NazwaDruzyny2LBL->Font =
			gcnew Drawing::Font(pfc->Families[0], 20);

		NazwaDruzyny2LBL->AutoSize = false;
		NazwaDruzyny2LBL->Size = Drawing::Size(250, 40);

		//NazwaDruzyny2LBL->Anchor =
		//	AnchorStyles::Top | AnchorStyles::Right;
		NazwaDruzyny2LBL->TextAlign = ContentAlignment::MiddleRight;


		PanelOdpowiedzi->Controls->Add(NazwaDruzyny2LBL);



		// ==========================
		// TREŒÆ PYTANIA
		// ==========================

		TrescPytaniaLBL2 = gcnew Label();

		TrescPytaniaLBL2->Text = TrescPytaniaLBL->Text;
		TrescPytaniaLBL2->Font = gcnew System::Drawing::Font(pfc->Families[0], 30, FontStyle::Regular);
		TrescPytaniaLBL2->ForeColor = Color::Yellow;

		TrescPytaniaLBL2->Size = Drawing::Size(400, 80);
		TrescPytaniaLBL2->Location = Point(300, 20);

		TrescPytaniaLBL2->BackColor = Color::Black;
		TrescPytaniaLBL2->ForeColor = Color::Yellow;

		TrescPytaniaLBL2->Font =
			gcnew Drawing::Font(pfc->Families[0], 20);

		TrescPytaniaLBL2->TextAlign =
			System::Drawing::ContentAlignment::MiddleCenter;

		PanelOdpowiedzi->Controls->Add(TrescPytaniaLBL2);

		odpNr = gcnew cli::array<Label^>(5);
		odpHaslo = gcnew cli::array<Label^>(5);
		odpPunkty = gcnew cli::array<Label^>(5);

		int startY = 150;
		int odstep = 60;
		for (int i = 0; i < 5; i++)
		{
			// ==========================
			// NUMER ODPOWIEDZI
			// ==========================
			odpNr[i] = gcnew Label();
			odpNr[i]->Text = (i + 1).ToString() + ".";
			odpNr[i]->Size = Drawing::Size(50, 50);
			odpNr[i]->Location = Point(220, startY + (i * odstep));
			odpNr[i]->BackColor = Color::Black;
			odpNr[i]->ForeColor = Color::Yellow;
			odpNr[i]->Font =
				gcnew Drawing::Font(pfc->Families[0], 18);

			odpNr[i]->TextAlign =
				System::Drawing::ContentAlignment::MiddleCenter;

			// ==========================
			// UKRYTA ODPOWIED
			// ==========================
			odpHaslo[i] = gcnew Label();
			odpHaslo[i]->Text = "........................";
			odpHaslo[i]->Size = Drawing::Size(350, 50);
			odpHaslo[i]->Location = Point(300, startY + (i * odstep));

			odpHaslo[i]->BackColor = Color::Black;
			odpHaslo[i]->ForeColor = Color::Yellow;
			odpHaslo[i]->Font =
				gcnew Drawing::Font(pfc->Families[0], 18);

			odpHaslo[i]->TextAlign =
				System::Drawing::ContentAlignment::MiddleCenter;

			// ==========================
			// PUNKTY
			// ==========================
			odpPunkty[i] = gcnew Label();
			odpPunkty[i]->Text = "....";
			odpPunkty[i]->Size = Drawing::Size(80, 50);
			odpPunkty[i]->Location = Point(680, startY + (i * odstep));

			odpPunkty[i]->BackColor = Color::Black;
			odpPunkty[i]->ForeColor = Color::Yellow;
			odpPunkty[i]->Font =
				gcnew Drawing::Font(pfc->Families[0], 18);

			odpPunkty[i]->TextAlign =
				System::Drawing::ContentAlignment::MiddleCenter;

			PanelOdpowiedzi->Controls->Add(odpNr[i]);
			PanelOdpowiedzi->Controls->Add(odpHaslo[i]);
			PanelOdpowiedzi->Controls->Add(odpPunkty[i]);
		}

		OdpowiedzTB = gcnew TextBox();

		OdpowiedzTB->Text = "Wpisz odpowiedz";
		OdpowiedzTB->ForeColor = Color::Gray;
		OdpowiedzTB->Size = Drawing::Size(350, 50);
		OdpowiedzTB->Location = Point(300, 540);

		OdpowiedzTB->BackColor = Color::Black;
		OdpowiedzTB->ForeColor = Color::Yellow;
		OdpowiedzTB->BorderStyle = BorderStyle::None;
		OdpowiedzTB->Font =
			gcnew Drawing::Font(pfc->Families[0], 18);

		OdpowiedzTB->TextAlign =
			HorizontalAlignment::Center;

		OdpowiedzTB->Enter +=
			gcnew EventHandler(this, &MainWin::OdpowiedzTB_Enter);

		OdpowiedzTB->Leave +=
			gcnew EventHandler(this, &MainWin::OdpowiedzTB_Leave);

		OdpowiedzTB->KeyDown += gcnew KeyEventHandler(this, &MainWin::OdpowiedzTB_KeyDown);

		PanelOdpowiedzi->Controls->Add(OdpowiedzTB);

		SumaPunkty = gcnew Label();
		SumaPunkty->Text = "....";
		SumaPunkty->Size = Drawing::Size(80, 50);
		SumaPunkty->Location = Point(680, 540);
		SumaPunkty->BackColor = Color::Black;
		SumaPunkty->ForeColor = Color::Yellow;
		SumaPunkty->Font =
			gcnew Drawing::Font(pfc->Families[0], 18);

		SumaPunkty->TextAlign =
			System::Drawing::ContentAlignment::MiddleCenter;

		PanelOdpowiedzi->Controls->Add(SumaPunkty);

	}

	private: System::Void OdpowiedzTB_Enter(
		System::Object^ sender,
		System::EventArgs^ e)
	{
		if (OdpowiedzTB->Text == "Wpisz odpowiedz")
		{
			OdpowiedzTB->Text = "";
			OdpowiedzTB->ForeColor = Color::Yellow;
		}
	}

	private: System::Void OdpowiedzTB_Leave(
		System::Object^ sender,
		System::EventArgs^ e)
	{
		if (String::IsNullOrWhiteSpace(OdpowiedzTB->Text))
		{
			OdpowiedzTB->Text = "Wpisz odpowiedz";
			OdpowiedzTB->ForeColor = Color::Gray;
		}
	}
		   //	   private: System::Void PoleOdpowiedzi_Click(System::Object^ sender, System::EventArgs^ e) {
		   //		   TextBox^ kliknietePole = safe_cast<TextBox^>(sender);
		   //
		   //		   // Jeœli pole jest ukryte, "odkrywamy" je i pozwalamy na wpisanie odpowiedzi
		   //		   if (kliknietePole->ReadOnly == true) {
		   //			   kliknietePole->ReadOnly = false;
		   //			   kliknietePole->Text = ""; // Czyœcimy kropki
		   //			   kliknietePole->Focus();
		   // Ustawiamy kursor, ¿eby od razu pisaæ
		   //		   }
		   //	   }
		   //
		   //	private: System::Void PoleOdpowiedzi_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		   //		if (e->KeyCode == Keys::Enter) {
		   //			TextBox^ pole = safe_cast<TextBox^>(sender);
		   //			pole->ReadOnly = true; 
		   //			e->SuppressKeyPress = true; 
		   //			// TUTAJ SPRAWDZIC CZY ODPOWIEDZ JEST PRAWIDLOWA 
		   //
		   //		}
		   //	}
		   //
		   //private: System::Void PoleOdpowiedzi_Leave(System::Object^ sender, System::EventArgs^ e) {
		   //	TextBox^ pole = safe_cast<TextBox^>(sender);
		   //
		   //	if (String::IsNullOrWhiteSpace(pole->Text))
		   //	{
		   //		pole->Text = "............";
		   //		pole->ReadOnly = true;
		   //	}
		   //}
	private: System::Void InicjalizujBledy() {
		bledyLewo = gcnew cli::array<Label^>(3);
		bledyPrawo = gcnew cli::array<Label^>(3);

		for (int i = 0; i < 3; i++)
		{
			// LEWA STRONA
			bledyLewo[i] = gcnew Label();
			bledyLewo[i]->Text = "X";
			bledyLewo[i]->ForeColor = Color::Red;
			bledyLewo[i]->BackColor = Color::Transparent;
			bledyLewo[i]->Font = gcnew System::Drawing::Font(pfc->Families[0], 48);

			bledyLewo[i]->AutoSize = true;
			bledyLewo[i]->Location = Point(40, 240 + (i * 90));
			bledyLewo[i]->Visible = false;

			// PRAWA STRONA
			bledyPrawo[i] = gcnew Label();
			bledyPrawo[i]->Text = "X";
			bledyPrawo[i]->ForeColor = Color::Red;
			bledyPrawo[i]->BackColor = Color::Transparent;
			bledyPrawo[i]->Font = gcnew System::Drawing::Font(pfc->Families[0], 48);

			bledyPrawo[i]->AutoSize = true;
			bledyPrawo[i]->Location = Point(850, 240 + (i * 90));
			bledyPrawo[i]->Visible = false;

			PanelOdpowiedzi->Controls->Add(bledyLewo[i]);
			PanelOdpowiedzi->Controls->Add(bledyPrawo[i]);

		}
		// MessageBox::Show("Bledy zainicjalizowane");
	}

	private: System::Void DodajBlad()
	{
		// Pytamy silnik o to, kto jest aktywny (mamy pewnoœæ, ¿e wie)
		Druzyna aktywna = silnikGry->PobierzAktywnaDruzyne();
		int indeks = silnikGry->PobierzBledyAktywnejDruzyny();

		if (indeks < 3) {
			if (aktywna == Druzyna::Lewa) {
				bledyLewo[indeks]->Visible = true;
			}
			else {
				bledyPrawo[indeks]->Visible = true;
			}
		}

		bool czyZmianaDruzyny = silnikGry->DodajBladAktywnejDruzynie();

		// Automatyczna zmiana po 3 b³êdach
		if (czyZmianaDruzyny)
		{
			if (aktywna == Druzyna::Lewa) {
				UstawAktywnaDruzyne(Druzyna::Prawa);
			}
			else {
				UstawAktywnaDruzyne(Druzyna::Lewa);
			}
		}
	}

	private: System::Void ResetujBledy()
	{
		silnikGry->getDruzynaLewa()->ResetujBledy();
		silnikGry->getDruzynaPrawa()->ResetujBledy();

		for (int i = 0; i < 3; i++)
		{
			bledyLewo[i]->Visible = false;
			bledyPrawo[i]->Visible = false;
		}
	}

	private: System::Void UstawAktywnaDruzyne(Druzyna dr)
	{
		silnikGry->UstawAktywnaDruzyne(dr);

		if (dr == Druzyna::Prawa) {

			WskaznikDruzynyStrzalkaPrawa->Visible = true;
			WskaznikDruzynyStrzalkaLewa->Visible = false;
			//MessageBox::Show("Tekst ustawiony na: " + WskaznikDruzynyStrzalka->Text);
		}
		else {
			WskaznikDruzynyStrzalkaPrawa->Visible = false;
			WskaznikDruzynyStrzalkaLewa->Visible = true;
			//MessageBox::Show("Tekst ustawiony na: " + WskaznikDruzynyStrzalka->Text);
		}
	}

	private: System::Void InicjalizujPanelFinal()
	{
		// U¿ywamy panelu stworzonego przez Designera
		PanelFinal->BackColor = Color::Black;
		PanelFinal->Visible = false;
		// =========================
		// NAZWA GRACZA
		// =========================
		FinalPlayerLbl = gcnew Label();
		FinalPlayerLbl->Text = "GRACZ 1";
		FinalPlayerLbl->ForeColor = Color::Yellow;
		FinalPlayerLbl->BackColor = Color::Black;
		FinalPlayerLbl->AutoSize = true;
		FinalPlayerLbl->Location = Point(40, 30);

		if (pfc->Families->Length > 0)
		{
			FinalPlayerLbl->Font =
				gcnew System::Drawing::Font(
					pfc->Families[0],
					28,
					FontStyle::Regular);
		}

		// =========================
		// LICZNIK CZASU
		// =========================
		FinalTimerLbl = gcnew Label();
		FinalTimerLbl->Text = "20";
		FinalTimerLbl->ForeColor = Color::Yellow;
		FinalTimerLbl->BackColor = Color::Black;
		FinalTimerLbl->AutoSize = true;
		FinalTimerLbl->Location = Point(850, 30);

		if (pfc->Families->Length > 0)
		{
			FinalTimerLbl->Font =
				gcnew System::Drawing::Font(
					pfc->Families[0],
					36,
					FontStyle::Regular);
		}

		// =========================
		// PYTANIE
		// =========================
		FinalQuestionLbl = gcnew Label();
		FinalQuestionLbl->Text = "PYTANIE";
		FinalQuestionLbl->ForeColor = Color::Yellow;
		FinalQuestionLbl->BackColor = Color::Black;
		FinalQuestionLbl->Size = Drawing::Size(800, 150);
		FinalQuestionLbl->Location = Point(80, 150);
		FinalQuestionLbl->TextAlign = ContentAlignment::MiddleCenter;
		if (pfc->Families->Length > 0)
		{
			FinalQuestionLbl->Font =
				gcnew System::Drawing::Font(
					pfc->Families[0],
					32,
					FontStyle::Regular);
		}

		// =========================
		// POLE ODPOWIEDZI
		// =========================
		FinalAnswerTB = gcnew TextBox();

		FinalAnswerTB->Location = Point(180, 380);
		FinalAnswerTB->Size = Drawing::Size(600, 60);

		FinalAnswerTB->Text = "............";

		FinalAnswerTB->Multiline = true;
		FinalAnswerTB->ReadOnly = true;

		FinalAnswerTB->BackColor = Color::Black;
		FinalAnswerTB->ForeColor = Color::Yellow;
		FinalAnswerTB->BorderStyle = BorderStyle::None;
		FinalAnswerTB->TextAlign = HorizontalAlignment::Center;

		if (pfc->Families->Length > 0)
		{
			FinalAnswerTB->Font =
				gcnew System::Drawing::Font(
					pfc->Families[0],
					28,
					FontStyle::Regular);
		}

		FinalAnswerTB->Click +=
			gcnew EventHandler(this, &MainWin::FinalAnswerTB_Click);

		FinalAnswerTB->Leave +=
			gcnew EventHandler(this, &MainWin::FinalAnswerTB_Leave);
		FinalAnswerTB->KeyDown +=
			gcnew KeyEventHandler(this, &MainWin::FinalAnswerTB_KeyDown);

		// Enter = nastêpne pytanie
		FinalAnswerTB->KeyDown +=
			gcnew KeyEventHandler(
				this,
				&MainWin::FinalAnswerTB_KeyDown);

		// =========================
		// DODANIE DO PANELU
		// =========================
		PanelFinal->Controls->Add(FinalPlayerLbl);
		PanelFinal->Controls->Add(FinalTimerLbl);
		PanelFinal->Controls->Add(FinalQuestionLbl);
		PanelFinal->Controls->Add(FinalAnswerTB);
	}

	private: System::Void PokazPytanieFinalowe()
	{
		FinalQuestionLbl->Text =
			finalQuestions[finalQuestionIndex];
	}

	private: System::Void FinalAnswerTB_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
	{
		if (e->KeyCode == Keys::Enter)
		{
			finalQuestionIndex++;
			if (finalQuestionIndex >= 5)
			{
				MessageBox::Show("Koniec pytan");
				return;
			}

			PokazPytanieFinalowe();

			FinalAnswerTB->Text = "............";
			FinalAnswerTB->ReadOnly = true;

			e->SuppressKeyPress = true;
		}
	}

	private: System::Void FinalAnswerTB_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (FinalAnswerTB->ReadOnly)
		{
			FinalAnswerTB->ReadOnly = false;
			FinalAnswerTB->Text = "";
			FinalAnswerTB->Focus();
		}
	}

	private: System::Void FinalAnswerTB_Leave(System::Object^ sender, System::EventArgs^ e)
	{
		if (String::IsNullOrWhiteSpace(FinalAnswerTB->Text))
		{
			FinalAnswerTB->Text = "............";
			FinalAnswerTB->ReadOnly = true;
		}
	}

	private: System::Void SprawdzOdpowiedz() {
		std::string wpisana = msclr::interop::marshal_as<std::string>(OdpowiedzTB->Text);

		// Pytamy silnik gry, czy odpowiedŸ jest poprawna
		int trafionyIndeks = silnikGry->SprawdzOdpowiedz(wpisana);

		if (trafionyIndeks != -1) { // -1 oznacza b³¹d, inny numer to trafienie!

			// Pobieramy pytanie tylko po to, ¿eby zaktualizowaæ interfejs (UI)
			TPytanie& aktualne = silnikGry->getAktualnePytanie();
			auto& odpowiedzi = aktualne.getOdpowiedzi();

			// Odkrywamy pole na ekranie
			odpHaslo[trafionyIndeks]->Text = gcnew System::String(odpowiedzi[trafionyIndeks].tekst.c_str());
			odpPunkty[trafionyIndeks]->Text = odpowiedzi[trafionyIndeks].punkty.ToString();
			odpPunkty[trafionyIndeks]->Visible = true;

			// Aktualizujemy etykietê z punktami ca³ej rundy
			this->SumaPunkty->Text = silnikGry->PobierzPunktyRundy().ToString();
			OdpowiedzTB->Text = "";
		}
		else {
			// Pud³o
			DodajBlad();
			OdpowiedzTB->Text = "";
		}
	}
	
	private: System::Void OdpowiedzTB_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Enter) {
			SprawdzOdpowiedz();
			e->SuppressKeyPress = true; // Usuwa dŸwiêk "bip" przy enterze
		}
	}
};
};
