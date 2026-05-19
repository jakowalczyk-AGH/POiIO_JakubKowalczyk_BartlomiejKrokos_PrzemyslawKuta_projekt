#pragma once

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
			pfc = gcnew PrivateFontCollection();

			// Œcie¿ka do katalogu z zasobami (plik czcionki i obrazki)
			String^ assetsPath = System::IO::Path::Combine(Application::StartupPath, "Assets");

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

				txtOdliczanie->Font = gcnew System::Drawing::Font(pfc->Families[0], 48, FontStyle::Regular);

				LiczbaGraczyLbl->Font = gcnew System::Drawing::Font(pfc->Families[0], 20);

				// 4. Ustawienie wygl¹du (¿eby by³o jak w teleturnieju)
				FamLbl->ForeColor = Color::Yellow;
				GrajBtn->ForeColor = Color::Yellow;
				NazwaDruzyny1TB->ForeColor = Color::Yellow;
				NazwaDruzyny2TB->ForeColor = Color::Yellow;
				txtOdliczanie->ForeColor = Color::Yellow;
				LiczbaGraczyLbl->ForeColor = Color::Yellow;

				NazwaDruzyny1TB->BackColor = Color::Black;
				NazwaDruzyny2TB->BackColor = Color::Black;
				txtOdliczanie->BackColor = Color::Black;

				NazwaDruzyny1TB->BorderStyle = System::Windows::Forms::BorderStyle::None; // Ukrywa bia³¹ ramkê
				NazwaDruzyny2TB->BorderStyle = System::Windows::Forms::BorderStyle::None; // Ukrywa bia³¹ ramkê
				txtOdliczanie->BorderStyle = System::Windows::Forms::BorderStyle::None;

				NazwaDruzyny1TB->Text = "LEWA";
				NazwaDruzyny2TB->Text = "PRAWA";


				// --- KONFIGURACJA KAFELKÓW WYBORU (3, 4, 5, 6) ---
				array<Button^>^ przyciski = { btn3, btn4, btn5, btn6 };
				int startX = 374; // Pozycja X pierwszego przycisku (œrodek ekranu)
				int kafelekSize = 45; // Rozmiar boku kwadratu
				int odstep = 10;

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
					przyciski[i]->Click -= gcnew System::EventHandler(this, &MainWin::WybierzLiczbeGraczy_Click); // Na wszelki wypadek usuñ stare
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
			txtOdliczanie->Height = 400;             // Zwiêksz wysokoœæ, ¿eby tekst siê nie uci¹³
			txtOdliczanie->Width = 800;              // Zwiêksz szerokoœæ


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
			catch (Exception^ ex) {
				// Nie przerywamy dzia³ania aplikacji, pozostawiamy obrazki niezainicjalizowane
			}
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


	private:
		bool czyNasluchiwacBuzzerow = false;
private: System::Windows::Forms::Panel^ PanelOdpowiedzi;
	   bool buzzerZablokowany = false;












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
			this->PanelStart->Margin = System::Windows::Forms::Padding(2);
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
			this->PanelGracze->Margin = System::Windows::Forms::Padding(2);
			this->PanelGracze->Name = L"PanelGracze";
			this->PanelGracze->Size = System::Drawing::Size(960, 585);
			this->PanelGracze->TabIndex = 6;
			// 
			// DalejBtn
			// 
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
			this->NazwaDruzyny2TB->Location = System::Drawing::Point(614, 81);
			this->NazwaDruzyny2TB->Name = L"NazwaDruzyny2TB";
			this->NazwaDruzyny2TB->Size = System::Drawing::Size(334, 20);
			this->NazwaDruzyny2TB->TabIndex = 6;
			this->NazwaDruzyny2TB->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->NazwaDruzyny2TB->Click += gcnew System::EventHandler(this, &MainWin::NazwaDruzyny2TB_Click);
			this->NazwaDruzyny2TB->Leave += gcnew System::EventHandler(this, &MainWin::NazwaDruzyny2TB_Leave);
			// 
			// NazwaDruzyny1TB
			// 
			this->NazwaDruzyny1TB->Location = System::Drawing::Point(12, 82);
			this->NazwaDruzyny1TB->Name = L"NazwaDruzyny1TB";
			this->NazwaDruzyny1TB->Size = System::Drawing::Size(333, 20);
			this->NazwaDruzyny1TB->TabIndex = 5;
			this->NazwaDruzyny1TB->Click += gcnew System::EventHandler(this, &MainWin::NazwaDruzyny1TB_Click);
			this->NazwaDruzyny1TB->Leave += gcnew System::EventHandler(this, &MainWin::NazwaDruzyny1TB_Leave);
			// 
			// DruzynaPrawaLbl
			// 
			this->DruzynaPrawaLbl->Location = System::Drawing::Point(0, 0);
			this->DruzynaPrawaLbl->Name = L"DruzynaPrawaLbl";
			this->DruzynaPrawaLbl->Size = System::Drawing::Size(100, 23);
			this->DruzynaPrawaLbl->TabIndex = 7;
			// 
			// DruzynaLewaLbl
			// 
			this->DruzynaLewaLbl->Location = System::Drawing::Point(0, 0);
			this->DruzynaLewaLbl->Name = L"DruzynaLewaLbl";
			this->DruzynaLewaLbl->Size = System::Drawing::Size(100, 23);
			this->DruzynaLewaLbl->TabIndex = 8;
			// 
			// PanelPrawaDruzyna
			// 
			this->PanelPrawaDruzyna->AutoSize = true;
			this->PanelPrawaDruzyna->Location = System::Drawing::Point(587, 139);
			this->PanelPrawaDruzyna->Margin = System::Windows::Forms::Padding(2);
			this->PanelPrawaDruzyna->Name = L"PanelPrawaDruzyna";
			this->PanelPrawaDruzyna->Size = System::Drawing::Size(362, 435);
			this->PanelPrawaDruzyna->TabIndex = 2;
			// 
			// PanelLewaDruzyna
			// 
			this->PanelLewaDruzyna->AutoSize = true;
			this->PanelLewaDruzyna->Location = System::Drawing::Point(12, 139);
			this->PanelLewaDruzyna->Margin = System::Windows::Forms::Padding(2);
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
			this->TrescPytaniaLBL->AutoSize = true;
			this->TrescPytaniaLBL->Location = System::Drawing::Point(439, 152);
			this->TrescPytaniaLBL->Name = L"TrescPytaniaLBL";
			this->TrescPytaniaLBL->Size = System::Drawing::Size(35, 13);
			this->TrescPytaniaLBL->TabIndex = 2;
			this->TrescPytaniaLBL->Text = L"label1";
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
			// PanelOdpowiedzi
			// 
			this->PanelOdpowiedzi->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PanelOdpowiedzi->Location = System::Drawing::Point(0, 0);
			this->PanelOdpowiedzi->Name = L"PanelOdpowiedzi";
			this->PanelOdpowiedzi->Size = System::Drawing::Size(960, 585);
			this->PanelOdpowiedzi->TabIndex = 9;
			// 
			// MainWin
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(960, 585);
			this->Controls->Add(this->PanelOdpowiedzi);
			this->Controls->Add(this->PanelStart);
			this->Controls->Add(this->PanelPytanie);
			this->Controls->Add(this->PanelGracze);
			this->Controls->Add(this->PanelCzek);
			this->Controls->Add(this->pictureBox1);
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


		// Pokazujemy tylko to, co chcemy
		ekran->Visible = true;
		ekran->BringToFront(); // Wa¿ne, ¿eby wskoczy³ na wierzch

		if (ekran == PanelPytanie) {
			czyNasluchiwacBuzzerow = true;
			buzzerZablokowany = false; // Resetujemy stan przy wejœciu
		}
		else {
			czyNasluchiwacBuzzerow = false;
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
		   //		Box[i, 0]->Location = Point(10, i * 60); // Odstêp 60px w pionie
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
		//GrajBtn->Visible = false;
		//FamLbl->Visible = false;

		PokazEkran(PanelCzek);
		//PanelGracze->Visible = true;

		czasPozostaly = 6;

		timer1->Start();
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
		   buzzerZablokowany = false;
	   }


	private: System::Void MainWin_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		// 1. Jeœli buzzer jest ju¿ zablokowany, ignorujemy kolejne klawisze
		if (!czyNasluchiwacBuzzerow) return;
		if (buzzerZablokowany) return;

		// 2. Obs³uga pierwszej dru¿yny (np. klawisz Q)
		if (e->KeyCode == Keys::Q) {
			buzzerZablokowany = true;
			
			LewaDruzBuzzPB->Visible = false;
			LewaDruzzBuzzONpb->Visible = true;
			//LewaDruzBuzzPB->Image = imgLewoSwiecace;
			//LewaDruzBuzzPB->Refresh();
		MessageBox::Show("Lewa dru¿yna pierwsza!");
		}
		// 3. Obs³uga drugiej dru¿yny (np. klawisz P)
		else if (e->KeyCode == Keys::P) {
			buzzerZablokowany = true; 
			PrawaDruzBuzzPB->Visible = false;
			PrawaDruzzBuzzONpb->Visible = true;
			
			//PrawaDruzBuzzPB->Image = imgPrawoSwiecace;
			//PrawaDruzBuzzPB->Refresh();
				MessageBox::Show("Prawa dru¿yna pierwsza!");
		}
	}

	};
};