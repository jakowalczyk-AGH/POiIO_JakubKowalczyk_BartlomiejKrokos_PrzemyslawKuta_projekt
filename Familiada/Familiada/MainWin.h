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
			PrivateFontCollection^ pfc = gcnew PrivateFontCollection();

			try {
				// 2. £adujemy plik (upewnij siê, ¿e nazwa pliku jest identyczna!)
				pfc->AddFontFile("familiada.ttf");

				// 3. Sprawdzamy, czy siê za³adowa³a i przypisujemy do labela
				// pfc->Families[0] to nasza Familiada
				FamLbl->Font = gcnew System::Drawing::Font(pfc->Families[0], 96, FontStyle::Regular);
				GrajBtn->Font = gcnew System::Drawing::Font(pfc->Families[0], 48, FontStyle::Regular);

				// 4. Ustawienie wygl¹du (¿eby by³o jak w teleturnieju)
				FamLbl->ForeColor = Color::Yellow;
				GrajBtn->ForeColor = Color::Yellow;
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
			this->PlayersCountCB = (gcnew System::Windows::Forms::ComboBox());
			this->PanelLewaDruzyna = (gcnew System::Windows::Forms::Panel());
			this->PanelPrawaDruzyna = (gcnew System::Windows::Forms::Panel());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->PanelStart->SuspendLayout();
			this->PanelGracze->SuspendLayout();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::Black;
			this->pictureBox1->Location = System::Drawing::Point(16, 15);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(4);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(1280, 720);
			this->pictureBox1->TabIndex = 2;
			this->pictureBox1->TabStop = false;
			// 
			// GrajBtn
			// 
			this->GrajBtn->BackColor = System::Drawing::Color::Black;
			this->GrajBtn->Location = System::Drawing::Point(400, 567);
			this->GrajBtn->Margin = System::Windows::Forms::Padding(4);
			this->GrajBtn->Name = L"GrajBtn";
			this->GrajBtn->Size = System::Drawing::Size(439, 100);
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
			this->FamLbl->Location = System::Drawing::Point(70, 154);
			this->FamLbl->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->FamLbl->Name = L"FamLbl";
			this->FamLbl->Size = System::Drawing::Size(76, 16);
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
			this->PanelStart->Size = System::Drawing::Size(1280, 720);
			this->PanelStart->TabIndex = 5;
			this->PanelStart->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainWin::PanelStart_Paint);
			// 
			// PanelGracze
			// 
			this->PanelGracze->Controls->Add(this->PanelPrawaDruzyna);
			this->PanelGracze->Controls->Add(this->PanelLewaDruzyna);
			this->PanelGracze->Controls->Add(this->PlayersCountCB);
			this->PanelGracze->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PanelGracze->Location = System::Drawing::Point(0, 0);
			this->PanelGracze->Name = L"PanelGracze";
			this->PanelGracze->Size = System::Drawing::Size(1280, 720);
			this->PanelGracze->TabIndex = 6;
			// 
			// PlayersCountCB
			// 
			this->PlayersCountCB->FormattingEnabled = true;
			this->PlayersCountCB->Items->AddRange(gcnew cli::array< System::Object^  >(4) { L"3", L"4", L"5", L"6" });
			this->PlayersCountCB->Location = System::Drawing::Point(603, 154);
			this->PlayersCountCB->Name = L"PlayersCountCB";
			this->PlayersCountCB->Size = System::Drawing::Size(121, 24);
			this->PlayersCountCB->TabIndex = 0;
			this->PlayersCountCB->SelectedIndexChanged += gcnew System::EventHandler(this, &MainWin::PlayersCountCB_SelectedIndexChanged);
			// 
			// PanelLewaDruzyna
			// 
			this->PanelLewaDruzyna->Location = System::Drawing::Point(299, 171);
			this->PanelLewaDruzyna->Name = L"PanelLewaDruzyna";
			this->PanelLewaDruzyna->Size = System::Drawing::Size(200, 100);
			this->PanelLewaDruzyna->TabIndex = 1;
			// 
			// PanelPrawaDruzyna
			// 
			this->PanelPrawaDruzyna->Location = System::Drawing::Point(826, 171);
			this->PanelPrawaDruzyna->Name = L"PanelPrawaDruzyna";
			this->PanelPrawaDruzyna->Size = System::Drawing::Size(200, 100);
			this->PanelPrawaDruzyna->TabIndex = 2;
			// 
			// MainWin
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(1280, 720);
			this->Controls->Add(this->PanelGracze);
			this->Controls->Add(this->PanelStart);
			this->Controls->Add(this->pictureBox1);
			this->Margin = System::Windows::Forms::Padding(4);
			this->MaximizeBox = false;
			this->Name = L"MainWin";
			this->Text = L"Familiada";
			this->Load += gcnew System::EventHandler(this, &MainWin::MainWin_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->PanelStart->ResumeLayout(false);
			this->PanelStart->PerformLayout();
			this->PanelGracze->ResumeLayout(false);
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
		tb2->Text = "ZAWODNIK " + (i + 1);
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
};
}
