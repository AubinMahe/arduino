#include "UI_Impl.h"

using namespace ncurses;

UI::UI() :
   _ui( new UI_Impl())
{}

UI:: ~ UI() {
   delete _ui;
}

//-- Digital I/O ----------------------------------------------------------

int UI::digitalRead( uint8_t pin ) const {
   return _ui->ctrl().digitalRead( pin );
}

void UI::digitalWrite( uint8_t pin, uint8_t hiOrLo ) const {
   _ui->enqueue( [ this, pin, hiOrLo ] () { _ui->ctrl().digitalWrite( pin, hiOrLo ); });
}

void UI::pinMode( uint8_t pin, uint8_t mode ) const {
   _ui->enqueue( [ this, pin, mode ] () { _ui->ctrl().pinMode( pin, mode, _ui ); });
}

//-- Analog I/O -----------------------------------------------------------

int UI::analogRead( uint8_t pin ) const {
   return _ui->ctrl().analogRead( pin );
}

void UI::analogReference( uint8_t mode ) const {
   _ui->enqueue( [ this, mode ] () { _ui->ctrl().analogReference( mode ); });
}

void UI::analogWrite( uint8_t pin, int value ) const {
   _ui->enqueue( [ this, pin, value ] () { _ui->ctrl().analogWrite( pin, value ); });
}

//-- Advanced I/O ---------------------------------------------------------

void UI::noTone( uint8_t pin ) const {
   _ui->enqueue( [ this, pin ] () { _ui->ctrl().noTone( pin ); });
}

void UI::tone( uint8_t pin, unsigned int frequency, unsigned long duration ) const {
   _ui->enqueue( [ this, pin, frequency, duration ] () { _ui->ctrl().tone( pin, frequency, duration ); });
}

//-- External Interrupts --------------------------------------------------

void UI::attachInterrupt( uint8_t pin, void (* ISR )(void), uint8_t mode ) const {
   _ui->attachInterrupt( pin, ISR, mode );
}

void UI::detachInterrupt( uint8_t pin ) const {
   _ui->detachInterrupt( pin );
}

//-- Communication --------------------------------------------------------

void UI::print( const char * line ) const {
   _ui->enqueue( [ this, line ] () { _ui->log().print( line ); });
}

void UI::println( const char * line ) const {
   _ui->enqueue( [ this, line ] () { _ui->log().println( line ); });
}

//-- Servo -------------------------------------------------------------------

void UI::servoAttach( uint8_t pin ) const {
   _ui->enqueue( [this,pin] () { _ui->ctrl().servoAttach( pin ); });
}

void UI::servoWrite( uint8_t pin, int value ) const {
   _ui->enqueue( [this,pin,value]() { _ui->ctrl().servoWrite( pin, value ); });
}

void UI::servoDetach( uint8_t pin ) const {
   _ui->enqueue( [this,pin]() { _ui->ctrl().servoDetach( pin ); });
}
