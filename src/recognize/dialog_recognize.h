#pragma once
#include <string>
#include <map>
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/combobox.h>
namespace agi { struct Context; }

class RecognizeDialog : public wxDialog
{
public:
    RecognizeDialog(agi::Context *context);

private:
    void OnRecognizeButtonClick(wxCommandEvent& event);
    void OnComboChange(wxCommandEvent& event);

    std::vector<std::string> engine_list_;
    agi::Context *context_;

    wxComboBox *engine_combo_;
    wxTextCtrl *access_key_id_;
    wxTextCtrl *access_key_secret_;
    wxTextCtrl *app_key_;
    std::string previous_engine_;
};

