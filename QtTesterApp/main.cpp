#include "QtTesterApp.h"
#include <QtWidgets/QApplication>

#include "..\ImportExport\RigidModel\Readers\RigidModelReader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1\emp\emp_karl_franz\emp_karl_franz.rigid_model_v2)");
    rmv2::RigidModelReader reader;
    auto rmv2File = reader.Read(bytes);



    QtTesterApp w;
    w.show();
    return a.exec();
}
