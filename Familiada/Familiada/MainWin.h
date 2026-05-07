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

			try {
				// 2. £adujemy plik (upewnij siê, ¿e nazwa pliku jest identyczna!)
				pfc->AddFontFile("familiada.ttf");

				// 3. Sprawdzamy, czy siê za³adowa³a i przypisujemy do labela
				// pfc->Families[0] to nasza Familiada
				FamLbl->Font = gcnew System::Drawing::Font(pfc->Families[0], 96, FontStyle::Regular);
				GrajBtn->Font = gcnew System::Drawing::Font(pfc->Families[0], 48, FontStyle::Regular);
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
				NazwaDruzyny2TB->BorderStyle = System::Windows::Forms::BorderStyle::None; // Ukrywa bia³¹ ramkê

				NazwaDruzyny1TB->Text = "LEWA";
				NazwaDruzyny2TB->Text = "PRAWA";


				// --- KONFIGURACJA KAFELKÓW WYBORU (3, 4, 5, 6) ---
				array<Button^>^ przyciski = { btn3, btn4, btn5, btn6 };
				int startX = 374; // Pozycja X pierwszego przycisku (œrodek ekranu)
				int kafelekSize = 45; // Rozmiar boku kwadratu
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
					przyciski[i]->Click -= gcnew System::EventHandler(this, &MainWin::WybierzLiczbeGraczy_Click); // Na wszelki wypadek usuñ stare
					przyciski[i]->Click += gcnew System::EventHandler(this, &MainWin::WybierzLiczbeGraczy_Click);
				}


				this->BackColor = Color::Black;
			}
			catch (Exception^ ex) {
				// Jeœli pliku nie bêdzie, program siê nie wywali, tylko poka¿e b³¹d
				MessageBox::Show("Nie uda³o siê za³adowaæ czcionki: " + ex->Message);
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











	protected:

	protected:

	protected:

	private:
		/// <summary>
		/// Wymagana zmienna projektanta.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metoda wymagana do obs³ugi projektanta — nie nale¿y modyfikowaæ
		/// jej zawartoœci w edytorze kodu.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->GrajBtn = (gcnew System::Windows::Forms::Button());
			this->FamLbl = (gcnew System::Windows::Forms::Label());
			this->PanelStart = (gcnew System::Windows::Forms::Panel());
			this->PanelGracze = (gcnew System::Windows::Forms::Panel());
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->PanelStart->SuspendLayout();
			this->PanelGracze->SuspendLayout();
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
			this->GrajBtn->Location = System::Drawing::Point(300, 461);
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
			// 
			// NazwaDruzyny1TB
			// 
			this->NazwaDruzyny1TB->Location = System::Drawing::Point(12, 82);
			this->NazwaDruzyny1TB->Name = L"NazwaDruzyny1TB";
			this->NazwaDruzyny1TB->Size = System::Drawing::Size(333, 20);
			this->NazwaDruzyny1TB->TabIndex = 5;
			this->NazwaDruzyny1TB->Click += gcnew System::EventHandler(this, &MainWin::NazwaDruzyny1TB_Click);
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
			// MainWin
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(960, 585);
			this->Controls->Add(this->PanelGracze);
			this->Controls->Add(this->PanelStart);
			this->Controls->Add(this->pictureBox1);
			this->MaximizeBox = false;
			this->Name = L"MainWin";
			this->Text = L"Familiada";
			this->Load += gcnew System::EventHandler(this, &MainWin::MainWin_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->PanelStart->ResumeLayout(false);
			this->PanelStart->PerformLayout();
			this->PanelGracze->ResumeLayout(false);
			this->PanelGracze->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void MainWin_Load(System::Object^ sender, System::EventArgs^ e) {}

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
private: System::Void PanelStart_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
}

	   private: System::Void PokazEkran(Panel^ ekran) {
		   // Ukrywamy wszystko
		   PanelStart->Visible = false;
		   //PanelGracze->Visible = false;
		   //PanelPytanie->Visible = false;
		   //PanelOdpowiedz->Visible = false;
		   //PanelFinal->Visible = false;

		   // Pokazujemy tylko to, co chcemy
		   ekran->Visible = true;
		   ekran->BringToFront(); // Wa¿ne, ¿eby wskoczy³ na wierzch
	   }
private: System::Void PlayersCountCB_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	// 1. Pobieramy wybran¹ liczbê (np. 3, 4, 5 lub 6)
	int count = System::Convert::ToInt32(PlayersCountCB->SelectedItem);

	// 2. Czyœcimy stare pola, ¿eby siê nie nak³ada³y
	PanelLewaDruzyna->Controls->Clear();
	PanelPrawaDruzyna->Controls->Clear();

	for (int i = 0; i < count; i++) {
		// --- DRU¯YNA 1 (Lewa) ---
		TextBox^ tb1 = gcnew TextBox();
		tb1->Text = "GRACZ " + (i + 1);
		tb1->Width = 250;
		tb1->BackColor = Color::Black;
		tb1->ForeColor = Color::Yellow;
		tb1->Font = gcnew System::Drawing::Font(pfc->Families[0], 20); // Twoja czcionka!
		tb1->Location = Point(10, i * 60); // Odstêp 60px w pionie
		PanelLewaDruzyna->Controls->Add(tb1);

		// --- DRU¯YNA 2 (Prawa - Odbicie) ---
		TextBox^ tb2 = gcnew TextBox();
		tb2->Text = "GRACZ " + (i + 1);
		tb2->Width = 250;
		tb2->BackColor = Color::Black;
		tb2->ForeColor = Color::Yellow;
		tb2->Font = gcnew System::Drawing::Font(pfc->Families[0], 20);
		// Odbicie lustrzane: ustawiamy tekst do prawej
		tb2->TextAlign = HorizontalAlignment::Right;
		tb2->Location = Point(PanelPrawaDruzyna->Width - tb2->Width - 10, i * 60);
		PanelPrawaDruzyna->Controls->Add(tb2);
	}
}

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

	// 2. Wizualne zaznaczenie wyboru (opcjonalnie)
	array<Button^>^ przyciski = { btn3, btn4, btn5, btn6 };
	for each (Button ^ b in przyciski) {
		b->BackColor = Color::Black;
		b->ForeColor = Color::Yellow;
	}
	klikniety->BackColor = Color::Yellow;
	klikniety->ForeColor = Color::Black;

	// 3. Generowanie pól (Twoja istniej¹ca logika)
	PanelLewaDruzyna->Controls->Clear();
	PanelPrawaDruzyna->Controls->Clear();

	for (int i = 0; i < count; i++) {
		// Lewa strona
		TextBox^ tb1 = gcnew TextBox();
		tb1->Text = "GRACZ " + (i + 1);
		tb1->Width = 250;
		tb1->BackColor = Color::Black;
		tb1->ForeColor = Color::Yellow;
		tb1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		if (pfc->Families->Length > 0) tb1->Font = gcnew System::Drawing::Font(pfc->Families[0], 20);
		tb1->Location = Point(10, i * 60);
		PanelLewaDruzyna->Controls->Add(tb1);

		// Prawa strona
		TextBox^ tb2 = gcnew TextBox();
		tb2->Text = "GRACZ " + (i + 1);
		tb2->Width = 250;
		tb2->BackColor = Color::Black;
		tb2->ForeColor = Color::Yellow;
		tb2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		if (pfc->Families->Length > 0) tb2->Font = gcnew System::Drawing::Font(pfc->Families[0], 20);
		tb2->TextAlign = HorizontalAlignment::Right;
		tb2->Location = Point(PanelPrawaDruzyna->Width - tb2->Width - 10, i * 60);
		PanelPrawaDruzyna->Controls->Add(tb2);
	}
}
};
}
