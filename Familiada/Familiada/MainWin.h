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




			/*finalQuestions = gcnew cli::array<String^>(5);

			finalQuestions[0] = "Wymien owoc";
			finalQuestions[1] = "Wymien zwierzê";
			finalQuestions[2] = "Wymien kolor";
			finalQuestions[3] = "Wymien sport";
			finalQuestions[4] = "Wymien napój";*/


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

	private: System::Windows::Forms::Timer^ FinalTimer;


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
		int finalQuestionIndex = 0;
		private: int aktualnyGraczFinalowy = 1;
		private: cli::array<String^>^ odpGracz1;
		private: cli::array<int>^ pktGracz1;
		private: cli::array<String^>^ odpGracz2;
		private: cli::array<int>^ pktGracz2;
		private: String^ nazwaGracza1;
		private: String^ nazwaGracza2;

	private: System::Windows::Forms::TextBox^ OdpowiedzTB;

	private:
		cli::array<Label^>^ odpNr;
		cli::array<Label^>^ odpHaslo;
		cli::array<Label^>^ odpPunkty;

	private: Label^ PunktyDruzyny1LBL;
	private: Label^ PunktyDruzyny2LBL;

	private: Label^ NazwaDruzyny1LBL;
	private: Label^ NazwaDruzyny2LBL;

	private: TGra* silnikGry;

	private:
		int czasPozostaly = 6;
		int finalCzasPozostaly = 40;

		bool czyMiedzyPytaniami = false;
		bool przedFinalem = false;
private: System::Windows::Forms::Timer^ timer2;
	   int numerPytania = 1;










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
			this->FinalTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->PanelOdpowiedzi = (gcnew System::Windows::Forms::Panel());
			this->PanelFinal = (gcnew System::Windows::Forms::Panel());
			this->timer2 = (gcnew System::Windows::Forms::Timer(this->components));
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
			this->pictureBox1->Location = System::Drawing::Point(12, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(960, 585);
			this->pictureBox1->TabIndex = 2;
			this->pictureBox1->TabStop = false;
			// 
			// GrajBtn
			// 
			this->GrajBtn->BackColor = System::Drawing::Color::Black;
			this->GrajBtn->Location = System::Drawing::Point(315, 461);
			this->GrajBtn->Name = L"GrajBtn";
			this->GrajBtn->Size = System::Drawing::Size(329, 81);
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
			this->FamLbl->Location = System::Drawing::Point(52, 125);
			this->FamLbl->Name = L"FamLbl";
			this->FamLbl->Size = System::Drawing::Size(63, 13);
			this->FamLbl->TabIndex = 3;
			this->FamLbl->Text = L"FAMILIADA";
			// 
			// PanelStart
			// 
			this->PanelStart->Controls->Add(this->FamLbl);
			this->PanelStart->Controls->Add(this->GrajBtn);
			this->PanelStart->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PanelStart->Location = System::Drawing::Point(0, 0);
			this->PanelStart->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->PanelStart->Name = L"PanelStart";
			this->PanelStart->Size = System::Drawing::Size(960, 585);
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
			this->PanelGracze->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->PanelGracze->Name = L"PanelGracze";
			this->PanelGracze->Size = System::Drawing::Size(960, 585);
			this->PanelGracze->TabIndex = 6;
			// 
			// DalejBtn
			// 
			this->DalejBtn->ForeColor = System::Drawing::Color::Yellow;
			this->DalejBtn->BackColor = System::Drawing::Color::Black;
			this->DalejBtn->Location = System::Drawing::Point(315, 461);
			this->DalejBtn->Name = L"DalejBtn";
			this->DalejBtn->Size = System::Drawing::Size(329, 81);
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
			this->LiczbaGraczyLbl->Location = System::Drawing::Point(360, 30);
			this->LiczbaGraczyLbl->Name = L"LiczbaGraczyLbl";
			this->LiczbaGraczyLbl->Size = System::Drawing::Size(91, 13);
			this->LiczbaGraczyLbl->TabIndex = 14;
			this->LiczbaGraczyLbl->Text = L"LICZBA GRACZY";
			// 
			// btn6
			// 
			this->btn6->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btn6->Location = System::Drawing::Point(535, 79);
			this->btn6->Name = L"btn6";
			this->btn6->Size = System::Drawing::Size(25, 23);
			this->btn6->TabIndex = 12;
			this->btn6->Text = L"6";
			this->btn6->UseVisualStyleBackColor = true;
			// 
			// btn5
			// 
			this->btn5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btn5->Location = System::Drawing::Point(481, 82);
			this->btn5->Name = L"btn5";
			this->btn5->Size = System::Drawing::Size(27, 23);
			this->btn5->TabIndex = 11;
			this->btn5->Text = L"5";
			this->btn5->UseVisualStyleBackColor = true;
			// 
			// btn4
			// 
			this->btn4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btn4->Location = System::Drawing::Point(440, 81);
			this->btn4->Name = L"btn4";
			this->btn4->Size = System::Drawing::Size(35, 23);
			this->btn4->TabIndex = 10;
			this->btn4->Text = L"4";
			this->btn4->UseVisualStyleBackColor = true;
			// 
			// btn3
			// 
			this->btn3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btn3->Location = System::Drawing::Point(380, 82);
			this->btn3->Name = L"btn3";
			this->btn3->Size = System::Drawing::Size(54, 23);
			this->btn3->TabIndex = 9;
			this->btn3->Text = L"3";
			this->btn3->UseVisualStyleBackColor = true;
			this->btn3->Click += gcnew System::EventHandler(this, &MainWin::WybierzLiczbeGraczy_Click);
			// 
			// NazwaDruzyny2TB
			// 
			this->NazwaDruzyny2TB->Location = System::Drawing::Point(695, 80);
			this->NazwaDruzyny2TB->Name = L"NazwaDruzyny2TB";
			this->NazwaDruzyny2TB->Size = System::Drawing::Size(250, 20);
			this->NazwaDruzyny2TB->TabIndex = 6;
			this->NazwaDruzyny2TB->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->NazwaDruzyny2TB->Click += gcnew System::EventHandler(this, &MainWin::NazwaDruzyny2TB_Click);
			this->NazwaDruzyny2TB->Leave += gcnew System::EventHandler(this, &MainWin::NazwaDruzyny2TB_Leave);
			// 
			// NazwaDruzyny1TB
			// 
			this->NazwaDruzyny1TB->Location = System::Drawing::Point(25, 80);
			this->NazwaDruzyny1TB->Name = L"NazwaDruzyny1TB";
			this->NazwaDruzyny1TB->Size = System::Drawing::Size(250, 20);
			this->NazwaDruzyny1TB->TabIndex = 5;
			this->NazwaDruzyny1TB->Click += gcnew System::EventHandler(this, &MainWin::NazwaDruzyny1TB_Click);
			this->NazwaDruzyny1TB->Leave += gcnew System::EventHandler(this, &MainWin::NazwaDruzyny1TB_Leave);
			// 
			// DruzynaPrawaLbl
			// 
			this->DruzynaPrawaLbl->Location = System::Drawing::Point(0, 0);
			this->DruzynaPrawaLbl->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->DruzynaPrawaLbl->Name = L"DruzynaPrawaLbl";
			this->DruzynaPrawaLbl->Size = System::Drawing::Size(100, 23);
			this->DruzynaPrawaLbl->TabIndex = 7;
			// 
			// DruzynaLewaLbl
			// 
			this->DruzynaLewaLbl->Location = System::Drawing::Point(0, 0);
			this->DruzynaLewaLbl->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->DruzynaLewaLbl->Name = L"DruzynaLewaLbl";
			this->DruzynaLewaLbl->Size = System::Drawing::Size(100, 23);
			this->DruzynaLewaLbl->TabIndex = 8;
			// 
			// PanelPrawaDruzyna
			// 
			this->PanelPrawaDruzyna->AutoSize = true;
			this->PanelPrawaDruzyna->Location = System::Drawing::Point(587, 139);
			this->PanelPrawaDruzyna->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->PanelPrawaDruzyna->Name = L"PanelPrawaDruzyna";
			this->PanelPrawaDruzyna->Size = System::Drawing::Size(362, 435);
			this->PanelPrawaDruzyna->TabIndex = 2;
			// 
			// PanelLewaDruzyna
			// 
			this->PanelLewaDruzyna->AutoSize = true;
			this->PanelLewaDruzyna->Location = System::Drawing::Point(12, 139);
			this->PanelLewaDruzyna->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->PanelLewaDruzyna->Name = L"PanelLewaDruzyna";
			this->PanelLewaDruzyna->Size = System::Drawing::Size(362, 435);
			this->PanelLewaDruzyna->TabIndex = 1;
			// 
			// PlayersCountCB
			// 
			this->PlayersCountCB->Location = System::Drawing::Point(0, 0);
			this->PlayersCountCB->Name = L"PlayersCountCB";
			this->PlayersCountCB->Size = System::Drawing::Size(121, 21);
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
			this->PanelPytanie->Name = L"PanelPytanie";
			this->PanelPytanie->Size = System::Drawing::Size(960, 585);
			this->PanelPytanie->TabIndex = 7;
			// 
			// PrawaDruzzBuzzONpb
			// 
			this->PrawaDruzzBuzzONpb->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PrawaDruzzBuzzONpb.Image")));
			this->PrawaDruzzBuzzONpb->Location = System::Drawing::Point(753, 413);
			this->PrawaDruzzBuzzONpb->Name = L"PrawaDruzzBuzzONpb";
			this->PrawaDruzzBuzzONpb->Size = System::Drawing::Size(141, 129);
			this->PrawaDruzzBuzzONpb->TabIndex = 6;
			this->PrawaDruzzBuzzONpb->TabStop = false;
			this->PrawaDruzzBuzzONpb->Visible = false;
			// 
			// LewaDruzzBuzzONpb
			// 
			this->LewaDruzzBuzzONpb->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"LewaDruzzBuzzONpb.Image")));
			this->LewaDruzzBuzzONpb->Location = System::Drawing::Point(59, 413);
			this->LewaDruzzBuzzONpb->Name = L"LewaDruzzBuzzONpb";
			this->LewaDruzzBuzzONpb->Size = System::Drawing::Size(141, 129);
			this->LewaDruzzBuzzONpb->TabIndex = 5;
			this->LewaDruzzBuzzONpb->TabStop = false;
			this->LewaDruzzBuzzONpb->Visible = false;
			// 
			// PrawaDruzBuzzPB
			// 
			this->PrawaDruzBuzzPB->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PrawaDruzBuzzPB.Image")));
			this->PrawaDruzBuzzPB->Location = System::Drawing::Point(753, 413);
			this->PrawaDruzBuzzPB->Name = L"PrawaDruzBuzzPB";
			this->PrawaDruzBuzzPB->Size = System::Drawing::Size(140, 129);
			this->PrawaDruzBuzzPB->TabIndex = 4;
			this->PrawaDruzBuzzPB->TabStop = false;
			// 
			// LewaDruzBuzzPB
			// 
			this->LewaDruzBuzzPB->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"LewaDruzBuzzPB.Image")));
			this->LewaDruzBuzzPB->Location = System::Drawing::Point(59, 413);
			this->LewaDruzBuzzPB->Name = L"LewaDruzBuzzPB";
			this->LewaDruzBuzzPB->Size = System::Drawing::Size(141, 129);
			this->LewaDruzBuzzPB->TabIndex = 3;
			this->LewaDruzBuzzPB->TabStop = false;
			// 
			// TrescPytaniaLBL
			// 
			this->TrescPytaniaLBL->Location = System::Drawing::Point(83, 39);
			this->TrescPytaniaLBL->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->TrescPytaniaLBL->Name = L"TrescPytaniaLBL";
			this->TrescPytaniaLBL->Size = System::Drawing::Size(800, 520);
			this->TrescPytaniaLBL->TabIndex = 2;
			this->TrescPytaniaLBL->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// PanelCzek
			// 
			this->PanelCzek->Controls->Add(this->txtOdliczanie);
			this->PanelCzek->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PanelCzek->Location = System::Drawing::Point(0, 0);
			this->PanelCzek->Name = L"PanelCzek";
			this->PanelCzek->Size = System::Drawing::Size(960, 585);
			this->PanelCzek->TabIndex = 8;
			// 
			// txtOdliczanie
			// 
			this->txtOdliczanie->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->txtOdliczanie->Location = System::Drawing::Point(100, 80);
			this->txtOdliczanie->Name = L"txtOdliczanie";
			this->txtOdliczanie->ReadOnly = true;
			this->txtOdliczanie->Size = System::Drawing::Size(100, 13);
			this->txtOdliczanie->TabIndex = 0;
			// 
			// timer1
			// 
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &MainWin::timer1_Tick);
			// 
			// FinalTimer
			// 
			this->FinalTimer->Interval = 1000;
			this->FinalTimer->Tick += gcnew System::EventHandler(this, &MainWin::FinalTimer_Tick);
			// 
			// PanelOdpowiedzi
			// 
			this->PanelOdpowiedzi->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PanelOdpowiedzi->Location = System::Drawing::Point(0, 0);
			this->PanelOdpowiedzi->Name = L"PanelOdpowiedzi";
			this->PanelOdpowiedzi->Size = System::Drawing::Size(960, 585);
			this->PanelOdpowiedzi->TabIndex = 9;
			// 
			// PanelFinal
			// 
			this->PanelFinal->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PanelFinal->Location = System::Drawing::Point(0, 0);
			this->PanelFinal->Name = L"PanelFinal";
			this->PanelFinal->Size = System::Drawing::Size(960, 585);
			this->PanelFinal->TabIndex = 10;
			// 
			// MainWin
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(960, 585);
			this->Controls->Add(this->PanelStart);
			this->Controls->Add(this->PanelFinal);
			this->Controls->Add(this->PanelCzek);
			this->Controls->Add(this->PanelOdpowiedzi);
			this->Controls->Add(this->PanelPytanie);
			this->Controls->Add(this->PanelGracze);
			this->Controls->Add(this->pictureBox1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->KeyPreview = true;
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
			nazwaLewa = "Lewa";
		}
		if (nazwaPrawa == "" || nazwaPrawa == "PRAWA") {
			nazwaPrawa = "Prawa";
		}

		NazwaDruzyny1LBL->Text = gcnew System::String(nazwaLewa.c_str());
		NazwaDruzyny2LBL->Text = gcnew System::String(nazwaPrawa.c_str());

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

		int faktycznaLiczbaGraczy = silnikGry->getDruzynaLewa()->getGracze().size();
		silnikGry->UstawLiczbeRund(faktycznaLiczbaGraczy);

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

			//PanelGracze->Visible = true;

		PokazEkran(PanelCzek);

		timer1->Start();
		aktualnaSumaPunktow = 0;
		this->SumaPunkty->Text = "....";
	}


	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
		czasPozostaly--; czyMiedzyPytaniami;

		if (przedFinalem)
		{
			txtOdliczanie->Text =
				"FINA£\r\n\r\nROZPOCZNIE SIE ZA\r\n\r\n" +
				czasPozostaly.ToString();

			if (czasPozostaly <= 0)
			{
				timer1->Stop();
				PokazEkran(PanelFinal);
			}
		}
		else if (czyMiedzyPytaniami)
		{
			txtOdliczanie->Text = "RUNDA " +
				numerPytania.ToString() + "\r\n\r\n" + "PRZYGOTUJCIE SIE\r\n\r\n" +
				czasPozostaly.ToString();

			if (czasPozostaly <= 0)
			{
				timer1->Stop();
				PokazEkran(PanelPytanie);
			}
		}
		else
		{
			txtOdliczanie->Text =
				"ROZGRYWKA\r\n\r\nROZPOCZNIE SIE ZA\r\n\r\n" +
				czasPozostaly.ToString();

			if (czasPozostaly <= 0)
			{
				timer1->Stop();
				PokazEkran(PanelPytanie);
			}
		}


	}




	private: System::Void ResetBuzzerow() {
		LewaDruzBuzzPB->Image = imgLewoWygaszone;
		PrawaDruzBuzzPB->Image = imgPrawoWygaszone;
		silnikGry->OdblokujBuzzery();
	}


	private: System::Void MainWin_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
	{

		/*if (e->KeyCode == Keys::F)
		{
			finalQuestionIndex = 0;

			PokazPytanieFinalowe();

			FinalPlayerLbl->Text = "GRACZ 1";
			FinalTimerLbl->Text = "20";
			FinalAnswerTB->Clear();

			PokazEkran(PanelFinal);
			return;
		}*/
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
			e->SuppressKeyPress = true;

			if (silnikGry->WcisnijBuzzer(Druzyna::Lewa)) {
				LewaDruzBuzzPB->Visible = false;
				LewaDruzzBuzzONpb->Visible = true;

				PokazEkran(PanelOdpowiedzi);
				UstawAktywnaDruzyne(Druzyna::Lewa);

				silnikGry->UstawWlascicielaRundy(Druzyna::Lewa);
				silnikGry->UstawPrzejecieRundy(false);
			}
		}
		else if (e->KeyCode == Keys::P)
		{
			e->SuppressKeyPress = true;

			if (silnikGry->WcisnijBuzzer(Druzyna::Prawa)) {
				PrawaDruzBuzzPB->Visible = false;
				PrawaDruzzBuzzONpb->Visible = true;

				PokazEkran(PanelOdpowiedzi);
				UstawAktywnaDruzyne(Druzyna::Prawa);

				silnikGry->UstawWlascicielaRundy(Druzyna::Prawa);
				silnikGry->UstawPrzejecieRundy(false);
			}
		}
	}

	private: System::Void InicjalizujPanelOdpowiedzi()
	{

		// PUNKTY DRU¯YNY 1
		PunktyDruzyny1LBL = gcnew Label();
		PunktyDruzyny1LBL->Text = "0";
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
		PunktyDruzyny2LBL->Text = "0";
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
		NazwaDruzyny1LBL = gcnew Label();

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

		NazwaDruzyny2LBL = gcnew Label();

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

		int startY = 200;
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
		OdpowiedzTB->Location = Point(300, 530);

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
		SumaPunkty->Location = Point(680, 520);
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
			silnikGry->UstawPrzejecieRundy(true);

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

		   private: System::Void PanelFinal_VisibleChanged(System::Object^ sender, System::EventArgs^ e) {
			   if (PanelFinal->Visible) {
				   FinalAnswerTB->Select(); // Wybiera kontrolkê
				   FinalAnswerTB->Focus();  // Wymusza fokus
			   }
		   }
	private: System::Void InicjalizujPanelFinal()
	{
		PanelFinal->VisibleChanged += gcnew EventHandler(this, &MainWin::PanelFinal_VisibleChanged);

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
		FinalAnswerTB->ReadOnly = false;

		FinalAnswerTB->BackColor = Color::Black;
		FinalAnswerTB->ForeColor = Color::Yellow;
		FinalAnswerTB->BorderStyle = BorderStyle::None;
		FinalAnswerTB->TextAlign = HorizontalAlignment::Center;
		FinalAnswerTB->TabIndex = 0;

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
		auto pytania = silnikGry->PobierzPytaniaFinalowe();

		if (finalQuestionIndex < pytania.size())
		{
			System::String^ trescStr =
				gcnew System::String(
					pytania[finalQuestionIndex].getTresc().c_str());

			FinalQuestionLbl->Text = trescStr;
		}
		//FinalTimerLbl->Text = "20";

		FinalTimer->Stop();
		FinalTimer->Start();
	}

	//private: System::Void FinalAnswerTB_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
	//{
	//	if (e->KeyCode == Keys::Enter)
	//	{
	//		// 1. Zapisujemy wpisan¹ odpowiedŸ i wyliczamy punkty
	//		String^ wpisana = FinalAnswerTB->Text;
	//		std::string wpisanaStd = msclr::interop::marshal_as<std::string>(wpisana);
	//		int pkt = silnikGry->SprawdzOdpowiedzFinalowa(finalQuestionIndex, wpisanaStd);

	//		// Zabezpieczenie: jeœli to koniec czasu, symulowany Enter móg³by zapisaæ "............" jako odpowiedŸ
	//		if (finalCzasPozostaly <= 0 && FinalAnswerTB->Text == "............") {
	//			wpisana = "BRAK CZASU";
	//			pkt = 0;
	//		}

	//		// 2. Wrzucamy do odpowiedniego worka
	//		if (aktualnyGraczFinalowy == 1) {
	//			odpGracz1[finalQuestionIndex] = wpisana;
	//			pktGracz1[finalQuestionIndex] = pkt;
	//		}
	//		else {
	//			odpGracz2[finalQuestionIndex] = wpisana;
	//			pktGracz2[finalQuestionIndex] = pkt;
	//		}

	//		// Przechodzimy do kolejnego pytania
	//		finalQuestionIndex++;

	//		// 3. Sprawdzamy czy to koniec tury gracza (odpowiedzia³ na 5 pytañ LUB skoñczy³ mu siê czas)
	//		if (finalQuestionIndex >= 5 || finalCzasPozostaly <= 0)
	//		{
	//			e->SuppressKeyPress = true; // Ucisza beep

	//			if (aktualnyGraczFinalowy == 1) {
	//				// Koniec Gracza 1 - okienko z podsumowaniem
	//				PokazPodsumowanieGracza1();

	//				// Setup dla Gracza 2
	//				aktualnyGraczFinalowy = 2;
	//				finalCzasPozostaly = 50; // Reset zegara dla drugiego gracza
	//				finalQuestionIndex = 0;
	//				FinalPlayerLbl->Text = nazwaGracza2;
	//				PokazPytanieFinalowe();

	//				FinalAnswerTB->Text = "............";
	//				FinalAnswerTB->ReadOnly = true;
	//			}
	//			else {
	//				// Koniec Gracza 2 - okienko fina³owe ca³ej gry!
	//				PokazPodsumowanieKoncowe();
	//			}
	//			return; // Zakoñczenie metody, bo zmieniamy gracza lub koñczymy grê
	//		}

	//		// 4. Jeœli gramy dalej (index < 5 i jest czas), odœwie¿amy UI dla kolejnego pytania
	//		PokazPytanieFinalowe();
	//		FinalAnswerTB->Text = "............";
	//		FinalAnswerTB->ReadOnly = true;

	//		e->SuppressKeyPress = true;
	//	}
	//}

	private: System::Void FinalAnswerTB_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
	{
		if (e->KeyCode == Keys::Enter)
		{

			String^ wpisana = FinalAnswerTB->Text;

			// =========================================================
			// 1. SPRAWDZANIE DUPLIKATÓW DLA GRACZA 2
			// =========================================================
			if (aktualnyGraczFinalowy == 2 && finalCzasPozostaly > 0)
			{
				bool czyByla = false;
				for (int i = 0; i < 5; i++)
				{
					// Sprawdzamy ignoruj¹c wielkoœæ liter
					if (String::Equals(wpisana, odpGracz1[i], StringComparison::OrdinalIgnoreCase))
					{
						czyByla = true;
						break;
					}
				}

				if (czyByla)
				{
					// Wyœwietlamy ostrze¿enie, czyœcimy pole i przerywamy klawisz Enter
					MessageBox::Show("BZZZZ! Ta odpowiedŸ ju¿ pad³a! Musisz wymyœliæ coœ innego.", "Powtórka", MessageBoxButtons::OK, MessageBoxIcon::Warning);
					FinalAnswerTB->Text = "";
					e->SuppressKeyPress = true;
					return; // Wychodzimy st¹d, ¿eby gracz musia³ wpisaæ now¹ odpowiedŸ
				}
			}
			// =========================================================

			std::string wpisanaStd = msclr::interop::marshal_as<std::string>(wpisana);
			int pkt = silnikGry->SprawdzOdpowiedzFinalowa(finalQuestionIndex, wpisanaStd);

			// Zabezpieczenie na koniec czasu
			if (finalCzasPozostaly <= 0 && FinalAnswerTB->Text == "............") {
				wpisana = "BRAK CZASU";
				pkt = 0;
			}

			// 2. Wrzucamy do odpowiedniego worka
			if (aktualnyGraczFinalowy == 1) {
				odpGracz1[finalQuestionIndex] = wpisana;
				pktGracz1[finalQuestionIndex] = pkt;
			}
			else {
				odpGracz2[finalQuestionIndex] = wpisana;
				pktGracz2[finalQuestionIndex] = pkt;
			}

			finalQuestionIndex++;

			// 3. Sprawdzamy czy to koniec tury gracza
			if (finalQuestionIndex >= 5 || finalCzasPozostaly <= 0)
			{
				e->SuppressKeyPress = true;

				// ZATRZYMUJEMY ZEGAR W MOMENCIE UKOÑCZENIA PYTAÑ LUB CZASU!
				FinalTimer->Stop();

				if (aktualnyGraczFinalowy == 1) {
					PokazPodsumowanieGracza1();

					// Setup dla Gracza 2
					aktualnyGraczFinalowy = 2;
					finalCzasPozostaly = 50;
					finalQuestionIndex = 0;
					FinalPlayerLbl->Text = nazwaGracza2;

					// To wywo³anie od nowa wystartuje zatrzymany wy¿ej zegar
					PokazPytanieFinalowe();

					FinalAnswerTB->Focus();

					FinalAnswerTB->Text = "";
					FinalAnswerTB->ReadOnly = false;
				}
				else {
					// Po zatrzymaniu zegara pokazujemy ostateczne okno
					PokazPodsumowanieKoncowe();
				}
				return;
			}

			// 4. Jeœli gramy dalej, odœwie¿amy UI
			PokazPytanieFinalowe();
			FinalAnswerTB->Text = "";
			FinalAnswerTB->ReadOnly = false;

			FinalAnswerTB->Focus();

			e->SuppressKeyPress = true;
		}
	}

	private: System::Void PokazPodsumowanieGracza1() {
		String^ podsum = "WYNIKI GRACZA: " + nazwaGracza1 + "\n\n";
		int suma = 0;
		auto pytania = silnikGry->PobierzPytaniaFinalowe();

		for (int i = 0; i < 5; i++) {
			String^ pytanie = gcnew String(pytania[i].getTresc().c_str());
			podsum += (i + 1).ToString() + ". " + pytanie + "\n";
			podsum += "   OdpowiedŸ: " + odpGracz1[i] + " (" + pktGracz1[i] + " pkt)\n\n";
			suma += pktGracz1[i];
		}

		podsum += "ZDOBYTE PUNKTY: " + suma.ToString() + "\n\nTERAZ GRA: " + nazwaGracza2 + "!";
		MessageBox::Show(podsum, "Podsumowanie tury");
	}

	private: System::Void PokazPodsumowanieKoncowe() {
		String^ podsum = "WYNIKI FINA£U!\n\n";
		int suma1 = 0, suma2 = 0;
		auto pytania = silnikGry->PobierzPytaniaFinalowe();

		for (int i = 0; i < 5; i++) {
			String^ pytanie = gcnew String(pytania[i].getTresc().c_str());
			podsum += (i + 1).ToString() + ". " + pytanie + "\n";
			podsum += "   " + nazwaGracza1 + ": " + odpGracz1[i] + " (" + pktGracz1[i] + " pkt)\n";
			podsum += "   " + nazwaGracza2 + ": " + odpGracz2[i] + " (" + pktGracz2[i] + " pkt)\n\n";
			suma1 += pktGracz1[i];
			suma2 += pktGracz2[i];
		}

		int sumaCalkowita = suma1 + suma2;
		podsum += "=============================\n";
		podsum += "Punkty - " + nazwaGracza1 + ": " + suma1.ToString() + "\n";
		podsum += "Punkty - " + nazwaGracza2 + ": " + suma2.ToString() + "\n";
		podsum += "£¥CZNIE W FINALE: " + sumaCalkowita.ToString() + " punktów!\n";

		if (sumaCalkowita >= 200) {
			podsum += "\nGRATULACJE! WYGRYWACIE G£ÓWN¥ NAGRODÊ!";
		}
		else {
			podsum += "\nNiestety, zabrak³o punktów do g³ównej wygranej.";
		}

		MessageBox::Show(podsum, "KONIEC GRY");
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

	private: System::Void KoniecRundyUI() {
		// Odœwie¿enie tablic z punktami dynamicznie z silnika gry
		PunktyDruzyny1LBL->Text = silnikGry->getDruzynaLewa()->getPunkty().ToString();
		PunktyDruzyny2LBL->Text = silnikGry->getDruzynaPrawa()->getPunkty().ToString();

		// Zerowanie puli punktów na œrodku
		SumaPunkty->Text = "....";

		// Wyœwietlenie komunikatu (tu docelowo mo¿esz podpi¹æ przejœcie do kolejnego pytania)
		MessageBox::Show("Punkty przyznane! Runda " + silnikGry->PobierzAktualnaRunde().ToString() + " zakonczona.");

		// Przechodzimy do kolejnej rundy w silniku
		silnikGry->InkrementujRunde();

		// Sprawdzamy warunek koñca rund zasadniczych
		if (silnikGry->CzyKoniecGry()) {
			MessageBox::Show("Koniec rund zasadniczych! Przechodzimy do FINA£U.");

			// --- PRZYGOTOWANIE FINA£U ---
			silnikGry->LosujPytaniaFinalowe(); // Losujemy prawdziwe pytania z JSONa

			// Pobieramy zwyciêzców i imiona pierwszych dwóch graczy
			TDruzyna* zwyciezca = silnikGry->PobierzZwyciezce();
			std::vector<std::string> graczeZwyciezcy = zwyciezca->getGracze();

			// Jeœli dru¿yna ma wpisanych graczy, pobieramy dwóch pierwszych
			nazwaGracza1 = graczeZwyciezcy.size() > 0 ? gcnew String(graczeZwyciezcy[0].c_str()) : "Gracz 1";
			nazwaGracza2 = graczeZwyciezcy.size() > 1 ? gcnew String(graczeZwyciezcy[1].c_str()) : "Gracz 2";

			aktualnyGraczFinalowy = 1;
			finalQuestionIndex = 0;

			// Resetujemy tablice na odpowiedzi
			odpGracz1 = gcnew cli::array<String^>(5);
			pktGracz1 = gcnew cli::array<int>(5);
			odpGracz2 = gcnew cli::array<String^>(5);
			pktGracz2 = gcnew cli::array<int>(5);

			PokazPytanieFinalowe();
			FinalPlayerLbl->Text = nazwaGracza1; // Wyœwietlamy imiê!
			//FinalTimerLbl->Text = "20";
			FinalAnswerTB->Clear();
			przedFinalem = true;
			czasPozostaly = 5;

			PokazEkran(PanelCzek);
			timer1->Start();
		}
		else {
			// Jeœli to nie koniec, czyœcimy stó³ i gramy dalej
			RozpocznijNastepnaRunde();
		}
	}

	private: System::Void RozpocznijNastepnaRunde() {
		// 1. Reset wizualny tablicy odpowiedzi i punktów rundy
		for (int i = 0; i < 5; i++) {
			odpHaslo[i]->Text = "........................";
			odpPunkty[i]->Text = "....";
		}

		// 2. Reset b³êdów (czyœci krzy¿yki na UI oraz stany w strukturach TDruzyna)
		ResetujBledy();

		// 3. Reset wizualny buzzerów (przywrócenie domyœlnych, wygaszonych grafik)
		LewaDruzBuzzPB->Visible = true;
		LewaDruzzBuzzONpb->Visible = false;
		PrawaDruzBuzzPB->Visible = true;
		PrawaDruzzBuzzONpb->Visible = false;
		silnikGry->OdblokujBuzzery();

		// 4. Losowanie nowego pytania z bazy i podmiana napisów
		silnikGry->losujPytanie();
		TPytanie wylosowane = silnikGry->getAktualnePytanie();
		System::String^ trescStr = gcnew System::String(wylosowane.getTresc().c_str());

		TrescPytaniaLBL->Text = trescStr;
		TrescPytaniaLBL2->Text = trescStr;

		// 5. Powrót do ekranu odliczania (PanelCzek) przed wejœciem na buzzery


		czyMiedzyPytaniami = true;
		numerPytania++;

		PokazEkran(PanelCzek);

		czasPozostaly = 6;

		timer1->Start();

	}



	private: System::Void SprawdzOdpowiedz() {
		std::string wpisana = msclr::interop::marshal_as<std::string>(OdpowiedzTB->Text);
		int trafionyIndeks = silnikGry->SprawdzOdpowiedz(wpisana);

		if (trafionyIndeks != -1) {
			// --- TRAFIENIE ---
			TPytanie& aktualne = silnikGry->getAktualnePytanie();
			auto& odpowiedzi = aktualne.getOdpowiedzi();

			// Odkrywamy pole na ekranie
			odpHaslo[trafionyIndeks]->Text = gcnew System::String(odpowiedzi[trafionyIndeks].tekst.c_str());
			odpPunkty[trafionyIndeks]->Text = odpowiedzi[trafionyIndeks].punkty.ToString();
			odpPunkty[trafionyIndeks]->Visible = true;

			this->SumaPunkty->Text = silnikGry->PobierzPunktyRundy().ToString();
			OdpowiedzTB->Text = "";

			// Weryfikacja warunków koñca rundy
			if (silnikGry->CzyRundaPrzejeta()) {
				silnikGry->RozliczPunkty(true); // Udana kradzie¿
				KoniecRundyUI();
			}
			else if (aktualne.CzyWszystkieOdkryte()) {
				silnikGry->RozliczPunkty(true); // Wyczyszczono planszê bez kradzie¿y
				KoniecRundyUI();
			}
		}
		else {
			// --- PUD£O ---
			OdpowiedzTB->Text = "";

			if (silnikGry->CzyRundaPrzejeta()) {
				// Nieudana próba kradzie¿y - punkty wracaj¹ do w³aœciciela
				silnikGry->RozliczPunkty(false);
				KoniecRundyUI();
			}
			else {
				// Zwyk³y b³¹d w standardowym toku rundy
				DodajBlad();
			}
		}
	}

	private: System::Void OdpowiedzTB_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Enter) {
			SprawdzOdpowiedz();
			e->SuppressKeyPress = true; // Usuwa dŸwiêk "bip" przy enterze
		}
	}

		   private: System::Void FinalTimer_Tick(
			   System::Object^ sender,
			   System::EventArgs^ e)
		   {
			   finalCzasPozostaly--;

			   FinalTimerLbl->Text =
				   finalCzasPozostaly.ToString();

			   //if (finalCzasPozostaly == 0) aktualnyGraczFinalowy = 2;

			   if (finalCzasPozostaly <= 0)
			   {

				   FinalTimer->Stop();

				   FinalAnswerTB->ReadOnly = true;

				   MessageBox::Show(
					   "Koniec czasu!",
					   "Fina³",
					   MessageBoxButtons::OK,
					   MessageBoxIcon::Information);

				   FinalAnswerTB_KeyDown(
					   nullptr,
					   gcnew KeyEventArgs(Keys::Enter));
			   }
		   }
	};
};
