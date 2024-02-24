//Tarea 10
// Implementar la funcionalidad para que el nodo no reenvie el mensaje por el mismo puerto donde lo recibió.
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

// Declaración de la clase Txc10 que hereda de cSimpleModule
class Txc10 : public cSimpleModule
{
  protected:
    virtual void forwardMessage(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// Registra la clase Txc10 como un módulo en OMNeT++
Define_Module(Txc10);

// Inicialización del módulo
void Txc10::initialize()
{
    // Si el módulo es "tic" con índice 0, programa el envío inicial del mensaje
    if (getIndex() == 0) {
        // Inicializa el proceso programando el mensaje inicial como un evento propio.
        char msgname[20];
        sprintf(msgname, "tic-%d", getIndex());
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg); // Programa el mensaje para ser enviado en t=0
    }
}

// Manejo de los mensajes recibidos por el módulo
void Txc10::handleMessage(cMessage *msg)
{
    // Si el módulo es "tic" con índice 3, significa que el mensaje ha llegado a su destino final
    if (getIndex() == 3) {
        // El mensaje ha llegado a su destino, se imprime un mensaje y se elimina el mensaje
        EV << "Message " << msg << " arrived.\n";
        delete msg;
    }
    else {
        // El mensaje no ha llegado a su destino final, se reenvía a otro módulo `tic`
        forwardMessage(msg);
    }
}

// Función para reenviar un mensaje a otro módulo `tic`
void Txc10::forwardMessage(cMessage *msg)
{
    cGate *arrivalGate = msg->getArrivalGate(); // Obtiene la puerta de llegada del mensaje

    int n = gateSize("out");
    int k;

    // Si el mensaje fue recibido a través de una puerta (no es un automensaje)
    if (arrivalGate != nullptr) {
        int arrivalGateIndex = arrivalGate->getIndex(); // Obtiene el índice de la puerta de llegada
        do {
            k = intuniform(0, n - 1); // Elige una puerta de salida aleatoria
        } while (k == arrivalGateIndex); // Evita que elija el mismo puerto de llegada
    } else {
        // Si el mensaje fue un automensaje, elige una puerta de salida aleatoria
        k = intuniform(0, n - 1);
    }

    EV << "Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k); // Envía el mensaje a través de la puerta seleccionada
}

