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
	private: System::Windows::Forms::Label^ FamLbl;
	private: System::Windows::Forms::Button^ GrajBtn;

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
			this->FamLbl = (gcnew System::Windows::Forms::Label());
			this->GrajBtn = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::Black;
			this->pictureBox1->Location = System::Drawing::Point(12, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(1240, 657);
			this->pictureBox1->TabIndex = 2;
			this->pictureBox1->TabStop = false;
			// 
			// FamLbl
			// 
			this->FamLbl->AutoSize = true;
			this->FamLbl->ForeColor = System::Drawing::Color::Yellow;
			this->FamLbl->Location = System::Drawing::Point(200, 167);
			this->FamLbl->Name = L"FamLbl";
			this->FamLbl->Size = System::Drawing::Size(63, 13);
			this->FamLbl->TabIndex = 3;
			this->FamLbl->Text = L"FAMILIADA";
			// 
			// GrajBtn
			// 
			this->GrajBtn->BackColor = System::Drawing::Color::Black;
			this->GrajBtn->Location = System::Drawing::Point(465, 476);
			this->GrajBtn->Name = L"GrajBtn";
			this->GrajBtn->Size = System::Drawing::Size(329, 81);
			this->GrajBtn->TabIndex = 4;
			this->GrajBtn->Text = L"GRAJ";
			this->GrajBtn->UseVisualStyleBackColor = false;
			this->GrajBtn->MouseLeave += gcnew System::EventHandler(this, &MainWin::GrajBtn_MouseLeave);
			this->GrajBtn->MouseHover += gcnew System::EventHandler(this, &MainWin::GrajBtn_MouseHover);
			// 
			// MainWin
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(1264, 681);
			this->Controls->Add(this->GrajBtn);
			this->Controls->Add(this->FamLbl);
			this->Controls->Add(this->pictureBox1);
			this->MaximizeBox = false;
			this->Name = L"MainWin";
			this->Text = L"Familiada";
			this->Load += gcnew System::EventHandler(this, &MainWin::MainWin_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

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
};
}
