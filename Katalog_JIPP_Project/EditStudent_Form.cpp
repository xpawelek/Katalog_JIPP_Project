#include "EditStudent_Form.h"
#include <QMessageBox>
#include "ownNamespace.h"
#include <regex> 


EditStudent_Form::EditStudent_Form(int index, string passedFirstName, string passedSecondName, bool passedIsStillStudying, string passedStudyLevel, string passedStudyMode, string passedFieldOfStudy, QWidget* parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);

	ui.comboBox_stopnie->addItem("Licencjackie");
	ui.comboBox_stopnie->addItem("Inzynierskie");
	ui.comboBox_stopnie->addItem("Magisterskie");
	ui.comboBox_stopnie->addItem("Doktoranckie");

	ui.lineEdit_firstName->setText(QString::fromStdString(passedFirstName));
	ui.lineEdit_secondName->setText(QString::fromStdString(passedSecondName));
	if (passedIsStillStudying)
	{
		ui.checkBox_IfStillStudy->setChecked(true);
	}
	ui.comboBox_stopnie->setCurrentText(QString::fromStdString(passedStudyLevel));
	if (passedStudyMode == "Stacjonarne") {
		ui.rd_stacjonarnie->setChecked(true);
	}
	else {
		ui.rd_niestacjonarnie->setChecked(true);
	}
	ui.lineEdit_Field->setText(QString::fromStdString(passedFieldOfStudy));

	passedIndex = index;
}

EditStudent_Form::~EditStudent_Form()
{}


void EditStudent_Form::editButton_Clicked() {
	string firstName = ownNamespace::capitalizeFirstLetter(ui.lineEdit_firstName->text().toStdString());
	string secondName = ownNamespace::capitalizeFirstLetter(ui.lineEdit_secondName->text().toStdString());
	string fieldOfStudy = ownNamespace::capitalizeFirstLetter(ui.lineEdit_Field->text().toStdString());

	vector <string> errors = {};
	regex myStringsRegex("[a-zA-Z ]+");
	if (std::regex_match(firstName, myStringsRegex) == false || firstName.length() < 3)
	{
		errors.push_back("Imie zawiera niepoprawne dane.");
	}

	if (std::regex_match(secondName, myStringsRegex) == false || firstName.length() < 3)
	{
		errors.push_back("Nazwisko zawiera niepoprawne dane.");
	}

	if (std::regex_match(fieldOfStudy, myStringsRegex) == false)
	{
		errors.push_back("Kierunek studi\u00F3w zawiera niepoprawne dane.");
	}


	if (ui.comboBox_stopnie->currentIndex() == -1)
	{
		errors.push_back("Stopie\u0144 studi\u00F3w nie zosta\u0142 wybrany.");
	}

	if (ui.rd_niestacjonarnie->isChecked() == false && ui.rd_stacjonarnie->isChecked() == false) {
		errors.push_back("Nie wybrano trybu studiowania.");
	}

	if (errors.size() == 0)
	{
		string studyMode = ui.rd_stacjonarnie->isChecked() ? "Stacjonarnie" : "Niestacjonarnie";
		bool isStillStudying = ui.checkBox_IfStillStudy->isChecked() ? true : false;
		emit sendEditedDataBack(passedIndex, firstName, secondName, isStillStudying, ui.comboBox_stopnie->currentText().toStdString(), studyMode, fieldOfStudy);
		QMessageBox::information(this, "Sukces", "Dane zosta\u0142y zedytowane.");
		this->close();
	}
	else {
		string errorMessage;
		for (auto it = begin(errors); it != end(errors); ++it) {
			errorMessage += *it + " \n";
		}

		QMessageBox::information(this, "Popraw wprowadzone dane.", QString::fromStdString(errorMessage));
	}
}