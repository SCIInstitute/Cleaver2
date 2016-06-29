#include "MainWindow.h"

#include <Cleaver/Cleaver.h>
#include <Cleaver/ConstantField.h>
#include <Cleaver/InverseField.h>
#include <cstdio>
#include <fstream>
#include <QProgressDialog>
#include <QApplication>
#include <QCheckBox>
#include <NRRDTools.h>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent) {}

MainWindow::MainWindow(const QString &title)
{
  this->setWindowTitle(title);
  this->setMinimumWidth(1024);  
  // Create Menus/Windows
  this->createDockWindows();
  this->createActions();
  this->setCentralWidget(this->window_);
  this->createMenus();

  exePath_ = QFileInfo( QCoreApplication::applicationFilePath()).path().toStdString();

  std::ifstream path((exePath_ + "/.path").c_str());
  if(path.is_open()) {
    path >> lastPath_ >> scirun_path_ >> python_path_;
    path.close();
  }

}

MainWindow::~MainWindow() {
  std::ofstream path((exePath_ + "/.path").c_str());
  path << lastPath_ << std::endl << scirun_path_  << std::endl << python_path_;
  path.close();
}

void MainWindow::createDockWindows()
{
  this->window_ = this->createWindow("Mesh Window");
  m_dataManagerWidget = new DataManagerWidget(this);
  m_cleaverWidget = new CleaverWidget(this, m_dataManagerWidget, this->window_);
  m_sizingFieldWidget = new SizingFieldWidget(this, m_dataManagerWidget);
  m_meshViewOptionsWidget = new MeshViewOptionsWidget(this, this->window_);

  addDockWidget(Qt::LeftDockWidgetArea, m_sizingFieldWidget);
  addDockWidget(Qt::LeftDockWidgetArea, m_cleaverWidget);
  addDockWidget(Qt::RightDockWidgetArea, m_dataManagerWidget);
  addDockWidget(Qt::RightDockWidgetArea, m_meshViewOptionsWidget);
}


void MainWindow::createActions()
{
  // File Menu Actions
  importVolumeAct = new QAction(tr("Import &Volume"), this);
  importVolumeAct->setShortcut(tr("Ctrl+v"));
  connect(importVolumeAct, SIGNAL(triggered()), this, SLOT(importVolume()));

  importSizingFieldAct = new QAction(tr("Import Sizing &Field"), this);
  importSizingFieldAct->setShortcut(tr("Ctrl+f"));
  connect(importSizingFieldAct, SIGNAL(triggered()), this, SLOT(importSizingField()));
  importSizingFieldAct->setDisabled(true);

  importMeshAct = new QAction(tr("Import &Mesh"), this);
  importMeshAct->setShortcut(tr("Ctrl+m"));
  connect(importMeshAct, SIGNAL(triggered()), this, SLOT(importMesh()));

  closeAct = new QAction(tr("Clear Data"), this);
  closeAct->setEnabled(true);
  closeAct->setShortcut(tr("Ctrl+d"));

  exportAct = new QAction(tr("&Export Mesh"), this);
  exportAct->setShortcut(tr("Ctrl+S"));
  exportAct->setDisabled(true);
  connect(exportAct, SIGNAL(triggered()), this, SLOT(exportMesh()));
  
  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  // Edit Menu Actions
  removeExternalTetsAct = new QAction(tr("Remove &External Tets"), this);
  connect(removeExternalTetsAct, SIGNAL(triggered()), this, SLOT(removeExternalTets()));

  removeLockedTetsAct = new QAction(tr("Remove &Locked Tets"), this);
  connect(removeLockedTetsAct, SIGNAL(triggered()), this, SLOT(removeLockedTets()));

  // Compute Menu Actions
  computeAnglesAct = new QAction(tr("Dihedral Angles"), this);
  connect(computeAnglesAct, SIGNAL(triggered()), this, SLOT(computeMeshAngles()));

  // View Menu Actions
  resetCameraAct = new QAction(tr("&Reset Camera"), this);
  resetCameraAct->setDisabled(true);
  connect(resetCameraAct, SIGNAL(triggered()), this, SLOT(resetCamera()));

  saveCameraAct = new QAction(tr("&Save Camera"), this);
  saveCameraAct->setDisabled(true);
  connect(saveCameraAct, SIGNAL(triggered()), this, SLOT(saveCamera()));

  loadCameraAct = new QAction(tr("&Load Camera"), this);
  loadCameraAct->setDisabled(true);
  connect(loadCameraAct, SIGNAL(triggered()), this, SLOT(loadCamera()));


  // Tool Menu Actions
  cleaverAction = m_cleaverWidget->toggleViewAction();
  cleaverAction->setCheckable(true);
  meshViewOptionsAction = m_meshViewOptionsWidget->toggleViewAction();
  meshViewOptionsAction->setCheckable(true);

  sizingFieldAction = m_sizingFieldWidget->toggleViewAction();
  sizingFieldAction->setCheckable(true);

  // About Menu Actions
  aboutAct = new QAction(tr("&About"), this);
  aboutAct->setStatusTip(tr("Show the About box"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}


void MainWindow::createMenus()
{
  // Top Level Menus
  m_fileMenu = new QMenu(tr("&File"), this);
  m_editMenu = new QMenu(tr("&Edit"), this);
  m_computeMenu = new QMenu(tr("&Compute"), this);
  m_viewMenu = new QMenu(tr("&View"), this);
  m_toolsMenu = new QMenu(tr("&Tools"), this);
  m_helpMenu = new QMenu(tr("&Help"), this);

  // File Menu Actions
  m_fileMenu->addAction(importVolumeAct);
  m_fileMenu->addAction(importSizingFieldAct);
  m_fileMenu->addAction(importMeshAct);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(exportAct);
  //m_fileMenu->addAction(exportAct2);
  m_fileMenu->addSeparator();
  //m_fileMenu->addAction(closeAct);
  m_fileMenu->addAction(closeAct);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(exitAct);

  // Edit Menu Actions
  m_editMenu->addAction(removeExternalTetsAct);
  m_editMenu->addAction(removeLockedTetsAct);

  // Compute Menu Actions
  m_computeMenu->addAction(computeAnglesAct);

  // View Menu Actions
  m_viewMenu->addAction(resetCameraAct);
  m_viewMenu->addAction(saveCameraAct);
  m_viewMenu->addAction(loadCameraAct);

  // Tool Menu Actions
  m_toolsMenu->addAction(sizingFieldAction);
  m_toolsMenu->addSeparator();
  m_toolsMenu->addAction(cleaverAction);
  m_toolsMenu->addSeparator();
  m_toolsMenu->addAction(meshViewOptionsAction);
  m_toolsMenu->addSeparator();

  // Help Menu Actions
  m_helpMenu->addAction(aboutAct);

  // Add Menus To MenuBar
  menuBar()->addMenu(m_fileMenu);
  menuBar()->addMenu(m_editMenu);
  menuBar()->addMenu(m_computeMenu);
  menuBar()->addMenu(m_viewMenu);
  menuBar()->addMenu(m_toolsMenu);
  menuBar()->addMenu(m_helpMenu);
}

//--------------------------------------
// - removeExternalTets()
// This method grabs the current window
// and if it has a mesh, calls the method
// on the mesh to remove external tets.
//--------------------------------------
void MainWindow::removeExternalTets()
{
  cleaver::TetMesh *mesh = this->m_cleaverWidget->getMesher()->getTetMesh();
  cleaver::Volume  *volume = this->m_cleaverWidget->getMesher()->getVolume();
  if(mesh && volume)
  {
    cleaver::stripExteriorTets(mesh,volume,true);

    // recompute update adjacency
    mesh->constructFaces();
    mesh->constructBottomUpIncidences();

    this->window_->setMesh(mesh);      // trigger update
  }
}

//--------------------------------------
// - removeCementedTets()
// This method grabs the current window
// and if it has a mesh, calls the method
// on the mesh to remove cemented tets.
//--------------------------------------
void MainWindow::removeLockedTets()
{
  cleaver::TetMesh *mesh = this->m_cleaverWidget->getMesher()->getTetMesh();
  if(mesh)
  {
    mesh->removeLockedTets();   // make it so
    this->window_->setMesh(mesh);      // trigger update
  }
}

void MainWindow::computeMeshAngles()
{
  cleaver::TetMesh *mesh = this->m_cleaverWidget->getMesher()->getTetMesh();
  if(mesh)
  {
    mesh->computeAngles();
    std::cout << "Min Angle: " << mesh->min_angle << " degrees." << std::endl;
    std::cout << "Max Angle: " << mesh->max_angle << " degrees." << std::endl;
  }
}
//*************Custom file dialog for segmentation check
class MyFileDialog : public QFileDialog
{
  public:
    MyFileDialog(QWidget *, const QString& a,
        const QString& b, const QString& c);
    bool isSegmentation();
    QSize sizeHint() const;
  private:
    QCheckBox *segmentation_check_;
};

bool MyFileDialog::isSegmentation() {
  return this->segmentation_check_->isChecked();
}

MyFileDialog::MyFileDialog( QWidget *parent, const QString& a,
    const QString& b, const QString& c) :
  QFileDialog( parent, a, b, c),
  segmentation_check_(NULL)
{
  setOption(QFileDialog::DontUseNativeDialog,true);
  setFileMode(QFileDialog::ExistingFiles);
  QGridLayout* mainLayout = dynamic_cast<QGridLayout*>(layout());

  QHBoxLayout *hbl = new QHBoxLayout();

  // add some widgets
  segmentation_check_ = new QCheckBox("This is a segmentation file", this);
  segmentation_check_->setChecked(false);
  hbl->addWidget(segmentation_check_);

  int numRows = mainLayout->rowCount();

  // add the new layout to the bottom of mainLayout
  // and span all columns
  mainLayout->addLayout( hbl, numRows,0,1,-1);
}


QSize MyFileDialog::sizeHint() const
{
  return QSize(800,600);
}

//*********************END custom file dialog
void MainWindow::importVolume()
{
  QStringList fileNames;
  MyFileDialog dialog(this, tr("Select Indicator Functions"),
      QString::fromStdString(lastPath_), tr("NRRD (*.nrrd)"));
  if (dialog.exec())
    fileNames = dialog.selectedFiles();
  bool segmentation = dialog.isSegmentation();
  if(!fileNames.isEmpty()) {
    std::string file1 = (*fileNames.begin()).toStdString();
    auto pos = file1.find_last_of('/');
    lastPath_ = file1.substr(0,pos);
    bool add_inverse = false;

    std::vector<std::string> inputs;

    for(int i=0; i < fileNames.size(); i++){
      inputs.push_back(fileNames[i].toStdString());
    }

    QProgressDialog status(QString("Loading Indicator Functions..."), QString(), 0, 100, this);
    status.show();
    status.setWindowModality(Qt::WindowModal);
    status.setValue(5);
    QApplication::processEvents();
    std::cout << " Loading input fields:" << std::endl;
    std::vector<cleaver::AbstractScalarField*> fields;
    if(inputs.empty()){
      std::cerr << "No material fields or segmentation files provided. Terminating."
        << std::endl;
      return;
    } else if (segmentation) {
      if (inputs.size() > 1) {
        std::cerr << "WARNING: More than one inputs provided for segmentation." <<
          " Only the first input will be used." << std::endl;
      }
      fields = NRRDTools::segmentationToIndicatorFunctions(inputs[0]);
      status.setValue(90);
      QApplication::processEvents();
    } else {
      if (inputs.size() == 1) {
        add_inverse = true;
      }

      for (size_t i = 0; i < inputs.size(); i++) {
        std::cout << " - " << inputs[i] << std::endl;
      }
      status.setValue(10);
      QApplication::processEvents();

      fields = NRRDTools::loadNRRDFiles(inputs);

      status.setValue(70);
      QApplication::processEvents();
      if (fields.empty()) {
        std::cerr << "Failed to load image data. Terminating." << std::endl;
        return;
      } else if (add_inverse) {
        fields.push_back(new cleaver::InverseScalarField(fields[0]));
        fields.back()->setName(fields[0]->name() + "-inverse");
      }
      status.setValue(90);
      QApplication::processEvents();
    }
    // Add Fields to Data Manager
    this->m_dataManagerWidget->setIndicators(fields);
    cleaver::Volume *volume = new cleaver::Volume(fields);

    static int v = 0;
    std::string volumeName = std::string("Volume");
    if(v > 0){
      volumeName += std::string(" ") + QString::number(v).toStdString();
    }
    volume->setName(volumeName);
    status.setValue(95);
    QApplication::processEvents();

    this->m_dataManagerWidget->setVolume(volume);
    this->window_->setVolume(volume);

    m_cleaverWidget->resetCheckboxes();
    status.setValue(100);
    this->m_sizingFieldWidget->setCreateButtonEnabled(true);
    this->m_cleaverWidget->setMeshButtonEnabled(true);
  }
}

void MainWindow::importSizingField()
{
  cleaver::Volume  *volume = this->m_cleaverWidget->getMesher()->getVolume();

  QString fileName = QFileDialog::getOpenFileName(this, tr("Select Sizing Field"),
      QString::fromStdString(lastPath_), tr("NRRD (*.nrrd)"));

  if(!fileName.isEmpty())
  {
    std::string file1 = QString((*fileName.begin())).toStdString();
    auto pos = file1.find_last_of('/');
    lastPath_ = file1.substr(0,pos);
    std::vector<cleaver::AbstractScalarField*> sizingField =
      NRRDTools::loadNRRDFiles({ {fileName.toStdString()} });
    volume->setSizingField(sizingField[0]);
    std::cout << "Sizing Field Set" << std::endl;
  }
}

void MainWindow::importMesh()
{
  QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Import Tetgen Mesh"),
      QString::fromStdString(lastPath_), tr("Tetgen Mesh (*.node *.ele)"));
  if (fileNames.size() == 1) {
    //try to correct only 1 file added, either .node or .ele
    std::string f = fileNames.at(0).toStdString();
    f = f.substr(0,f.find_last_of("."));
    fileNames.clear();
    fileNames.push_back(QString::fromStdString(f + ".node"));
    fileNames.push_back(QString::fromStdString(f + ".ele"));
  }
  if(fileNames.size() == 2)
  {
    std::string elefilename;
    std::string nodefilename;

    for(int i=0; i < 2; i++)
    {
      std::string fn = fileNames[i].toStdString();
      if(fn.substr(fn.find_last_of(".") + 1) == "ele")
        elefilename = fn;
      else if(fn.substr(fn.find_last_of(".") + 1) == "node")
        nodefilename = fn;
      else
      {
        std::cerr << "Invalid input extension!\n" << std::endl;
        return;
      }
    }

    if(elefilename.length() > 0 && nodefilename.length() > 0)
    {
      cleaver::TetMesh *mesh =
        cleaver::TetMesh::createFromNodeElePair(nodefilename, elefilename,false);
      if(mesh == NULL){
        std::cerr << "Invalid Mesh" << std::endl;
        return;
      }


      mesh->constructFaces();
      mesh->constructBottomUpIncidences();
      mesh->imported = true;

      this->m_meshViewOptionsWidget->setShowCutsCheckboxEnabled(false);
      this->m_dataManagerWidget->setMesh(mesh);
      this->window_->setMesh(mesh);
    }
  }
  if (!fileNames.empty()) {
    std::string file1 = (*fileNames.begin()).toStdString();
    auto pos = file1.find_last_of('/');
    lastPath_ = file1.substr(0,pos);
  }
}


void MainWindow::exportField(cleaver::FloatField *field)
{
  std::cout << "Exporting Field!!" << std::endl;
  if(!field)
    return;

  QString ext;
  QString selectedFilter;
  std::string name = field->name() == "" ? "Untitled" : field->name();
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Field As"),
      (lastPath_ + "/" + name).c_str(), tr("NRRD (*.nrrd);"), &selectedFilter);

  QString filter1("NRRD (*.nrrd)");

  NRRDTools::saveNRRDFile(field, std::string(fileName.toLatin1()));
  if (fileName != "") {
    std::string file1 = fileName.toStdString();
    auto pos = file1.find_last_of('/');
    lastPath_ = file1.substr(0,pos);
  }
}

void MainWindow::exportMesh(cleaver::TetMesh *mesh)
{
  // If no mesh selected, get active window mesh
  if (!mesh)
    mesh = this->m_cleaverWidget->getMesher()->getTetMesh();

  // If still no mesh, return (TODO: Error MessageBox)
  if(!mesh)
    return;

  QString ext;
  QString selectedFilter;
  std::string name = mesh->name == "" ? "Untitled" : mesh->name;
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Mesh As"),
      (lastPath_ + "/" + name).c_str(),
      tr("Tetgen (*.node);;SCIRun (*.pts);;Surface PLY (*.ply);;Matlab (*.mat);;VTK Poly (*.vtk);;VTK Unstructured Grid (*.vtk)"), &selectedFilter);


  QString filter1("Tetgen (*.node)");
  QString filter2("SCIRun (*.pts)");
  QString filter3("Surface PLY (*.ply)");
  QString filter4("Matlab (*.mat)");
  QString filter5("VTK Unstructured Grid (*.vtk)");
  QString filter6("VTK Poly (*.vtk)");

  std::string f = fileName.toStdString();
  f = f.substr(0,f.rfind("."));

  if(selectedFilter == filter1){
    mesh->writeNodeEle(f, true, true);
  }
  else if(selectedFilter == filter2){
    mesh->writePtsEle(f, true);
  }
  else if(selectedFilter == filter3){
    mesh->writePly(f, true);
  }
  else if(selectedFilter == filter4){
    mesh->writeMatlab(f, true);
  }
  else if(selectedFilter == filter5){
    mesh->writeVtkUnstructuredGrid(f, true);
  }
  else if(selectedFilter == filter6){
    mesh->writeVtkPolyData(f, true);
  }
  if (fileName != "") {
    std::string file1 = fileName.toStdString();
    auto pos = file1.find_last_of('/');
    lastPath_ = file1.substr(0,pos);
  }
}

void MainWindow::resetCamera()
{
  if(this->window_ != NULL)
  {
    this->window_->resetView();
  }
}

void MainWindow::saveCamera()
{
  if(this->window_ != NULL)
  {
    this->window_->saveView();
  }
}

void MainWindow::loadCamera()
{
  if(this->window_ != NULL)
  {
    this->window_->loadView();
  }
}


void MainWindow::about()
{
  // TODO: Make this a better Modal Frame rather than MessageBox
  QMessageBox::about(this, tr("About Cleaver 2.0 Beta"),
      tr("<b>Cleaver 2.0 Beta</b><BR>"
        "<a href=\"http://www.sci.utah.edu/\">Scientific Computing & Imaging Institute</a><BR>"
        "<a href=\"http://www.cs.utah.edu/\">University of Utah, School of Computing</a><BR>"
        "<P><b>Author:</b> Jonathan Bronson"
        "<P>This program is provided AS IS with NO "
        "WARRANTY OF ANY KIND, INCLUDING THE WARRANTY"
        "OF DESIGN, MERCHANTABILITY AND FITNESS FOR A "
        "PARTICULAR PURPOSE."));
}

MeshWindow * MainWindow::createWindow(const QString &title)
{
    QGLFormat glFormat;
    glFormat.setVersion(3, 3);
    glFormat.setProfile(QGLFormat::CoreProfile); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers(true);
    MeshWindow *window = new MeshWindow(glFormat, this);
    std::cout << "Current Context: " << 
      window->format().majorVersion() << "." << 
      window->format().minorVersion() << ", CORE? " << 
      ((window->format().profile() == 1) ? "true" : "false") << std::endl;
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->showMaximized();
    return window;
}

void MainWindow::enableMeshedVolumeOptions() {

  this->m_meshViewOptionsWidget->setShowCutsCheckboxEnabled(true);
  this->exportAct->setEnabled(true);
}