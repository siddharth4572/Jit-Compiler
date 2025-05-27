#include "gui_backend.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H> 
#include <string>

// Function declarations
std::string compile_and_get_ir(const std::string& source_code);
std::string run_jit_and_get_output(const std::string& source_code);

Fl_Multiline_Input* source_input;
Fl_Multiline_Output* ir_output;
Fl_Multiline_Output* program_output;

void on_run_button(Fl_Widget*, void*) {
    std::string source_code = source_input->value();
    std::string ir = compile_and_get_ir(source_code);
    ir_output->value(ir.c_str());

    std::string output = run_jit_and_get_output(source_code);
    program_output->value(output.c_str());
}

int main() {
    const int window_width = 900;
    const int window_height = 700;
    const int margin = 10;
    const int label_height = 25;
    const int run_button_height = 30;
    const int output_height = 190;

    Fl_Window* window = new Fl_Window(window_width, window_height, "JIT Compiler GUI");
    window->color(fl_rgb_color(245, 245, 245));  // Light gray background

    // Source and IR Labels
    Fl_Box* source_label = new Fl_Box(margin, margin, (window_width / 2) - 2 * margin, label_height, "Source Code:");
    source_label->labelfont(FL_HELVETICA_BOLD);
    source_label->labelsize(14);
    source_label->labelcolor(FL_DARK_BLUE);

    Fl_Box* ir_label = new Fl_Box((window_width / 2) + margin, margin, (window_width / 2) - 2 * margin, label_height, "LLVM IR:");
    ir_label->labelfont(FL_HELVETICA_BOLD);
    ir_label->labelsize(14);
    ir_label->labelcolor(FL_DARK_BLUE);

    int top_section_height = window_height - output_height - label_height * 2 - run_button_height - 5 * margin;

    // Input and IR Boxes
    source_input = new Fl_Multiline_Input(margin, margin + label_height + margin, (window_width / 2) - 2 * margin, top_section_height);
    source_input->color(FL_WHITE);
    source_input->textcolor(FL_BLACK);
    source_input->textfont(FL_COURIER);
    source_input->textsize(14);

    ir_output = new Fl_Multiline_Output((window_width / 2) + margin, margin + label_height + margin, (window_width / 2) - 2 * margin, top_section_height);
    ir_output->color(FL_WHITE);
    ir_output->textcolor(FL_BLACK);
    ir_output->textfont(FL_COURIER);
    ir_output->textsize(14);

    // Run Button
    Fl_Button* run_btn = new Fl_Button((window_width / 2) - 50, margin + label_height + top_section_height + margin, 100, run_button_height, "Compile & Run");
    run_btn->color(fl_rgb_color(70, 130, 180));  // Steel blue
    run_btn->labelcolor(FL_WHITE);
    run_btn->labelfont(FL_HELVETICA_BOLD);
    run_btn->callback(on_run_button);

    // Output Label
    int output_label_y = run_btn->y() + run_button_height + margin;
    Fl_Box* output_label = new Fl_Box(margin, output_label_y, window_width - 2 * margin, label_height, "Program Output:");
    output_label->labelfont(FL_HELVETICA_BOLD);
    output_label->labelsize(14);
    output_label->labelcolor(FL_DARK_BLUE);

    // Output Box
    int program_output_y = output_label_y + label_height + margin;
    program_output = new Fl_Multiline_Output(margin, program_output_y, window_width - 2 * margin, output_height);
    program_output->color(FL_WHITE);
    program_output->textcolor(FL_BLACK);
    program_output->textfont(FL_COURIER);
    program_output->textsize(14);

    window->end();
    window->show();

    return Fl::run();
}

