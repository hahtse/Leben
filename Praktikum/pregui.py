#
#pregui.py
#
import sys
import string
import os
from PyQt4 import QtCore, QtGui, QtWebKit
import ConfigParser

#Gui-File importieren
from hcgui import Ui_Fenster



class PreProc(QtGui.QMainWindow):
    def __init__(self,parent = None):
        QtGui.QMainWindow.__init__(self,parent)
        self.ui = Ui_Fenster()
        self.ui.setupUi(self)
        self.statusBar().showMessage("Welcome!")
        self.templateDict = {"T1 MNI 1mm fullbrain": "/usr/share/lipsia/mni.v",
                             "EPI 2mm": "/usr/share/lipsia/epi_template_2mm.v",
                             "EPI 3mm":"/usr/share/lipsia/epi_template_3mm.v",         
                             "EPI 1.5mm":"/usr/share/lipsia/epi_template_1.5mm.v"
                            }
        for template, path in self.templateDict.iteritems():
            if(os.path.isfile(path)):
                self.ui.comboBox_template.addItem(template)
        self.ui.comboBox_template.setCurrentIndex(2) 
        self.helpDict = {"Lipsia help": "/usr/share/doc/lipsia/html/index.html" }
        for help, path in self.helpDict.iteritems():
            if(os.path.isfile(path)):
                self.ui.help_Box.addItem(help)
        self.show_help()
        self.interpolators = ["Linear", "BSpline", "NearestNeighbor"]
        for inter in self.interpolators:
            self.ui.comboBox_interpol_func.addItem(inter) 

        #list connections
        QtCore.QObject.connect(self.ui.b_add_image_file, QtCore.SIGNAL("clicked()"), self.add_data_to_list )
        QtCore.QObject.connect(self.ui.b_add_image_directory, QtCore.SIGNAL("clicked()"), self.add_dir_to_list )
        QtCore.QObject.connect(self.ui.b_remove_file, QtCore.SIGNAL("clicked()"), self.remove_from_list )
        QtCore.QObject.connect(self.ui.b_start_proc, QtCore.SIGNAL("clicked()"), self.start_processing )
        QtCore.QObject.connect(self.ui.b_output_directory, QtCore.SIGNAL("clicked()"), self.open_output_dir )
        QtCore.QObject.connect(self.ui.b_add_anatomical_image, QtCore.SIGNAL("clicked()"), self.add_anatomical_image )
        QtCore.QObject.connect(self.ui.b_view, QtCore.SIGNAL("clicked()"), self.view_template_image )
        QtCore.QObject.connect(self.ui.b_remove_anatomical_image, QtCore.SIGNAL("clicked()"), self.remove_anatomical_image )
        QtCore.QObject.connect(self.ui.b_save_settings, QtCore.SIGNAL("clicked()"), self.save_settings_to_file )
        QtCore.QObject.connect(self.ui.b_load_settings, QtCore.SIGNAL("clicked()"), self.load_settings )
        QtCore.QObject.connect(self.ui.comboBox_help, QtCore.SIGNAL("currentIndexChanged(int)"), self.show_help )
        QtCore.QObject.connect(self.ui.cb_atlas_registration, QtCore.SIGNAL("stateChanged(int)"), self.change_atlas_reg)

#Einzelnes File hinzufuegen
    def add_data_to_list(self):
        print "add_data_to_list"

#Verzeichnis hinzufuegen
    def add_dir_to_list(self):
        print "add_dir_to_list"        

#Aus Liste entfernen
    def remove_from_list(self):
        numberSelect = self.ui.listWidget.currentRow()
        self.myFileList.remove(self.ui.listWidget.item(numberSelect).text())
        try:
            del self.anatomicalImageDict[str(self.ui.listWidget.item(numberSelect).text())]
        except:
            pass
        self.ui.lv_input_files.takeItem(numberSelect)
        self.ui.lv_functional_anatomical_image.takeItem(numberSelect)

#Verarbeitung starten
    def start_processing(self):
        print "start_processing"

#Ausgabeverzeichnis waehlen
    def open_output_dir(self):
        print "open_output_dir"

#Anatomisches Bild hinzufuegen
    def add_anatomical_image(self):
        print "add_anatomical_image"

#Zeigt ausgewaehltes Templateimage an
    def view_template_image(self):
        imageToShow = self.templateDict[str(self.ui.comboBox_template.currentText())]
        os.system("vlv -in " +  imageToShow + " 2> tmpOut")
        os.remove("tmpOut")

#Anatomisches Bild entfernen
    def remove_anatomical_image(self):
        print "remove_anatomical_image"

#ConfigParser zum Speichern
    def save_settings_to_file(self):
        config = ConfigParser.RawConfigParser()
        config.add_section('Preprocessing Steps')
        config.set('Preprocessing Steps', 'atlas_registration', self.ui.cb_atlas_registration.isChecked())
        config.set('Preprocessing Steps', 'create_mask', self.ui.cb_create_mask.isChecked())
        config.set('Preprocessing Steps', 'debug_output', self.ui.cb_debug_output.isChecked())
        config.set('Preprocessing Steps', 'fieldmap_correction', self.ui.cb_fieldmap_correction.isChecked())
        config.set('Preprocessing Steps', 'high_pass', self.ui.cb_high_pass.isChecked())
        config.set('Preprocessing Steps', 'low_pass', self.ui.cb_low_pass.isChecked())
        config.set('Preprocessing Steps', 'movement_correction', self.ui.cb_movement_correction.isChecked())
        config.set('Preprocessing Steps', 'set_repetition', self.ui.cb_set_repetition.isChecked())
        config.set('Preprocessing Steps', 'show_mask', self.ui.cb_show_mask.isChecked())
        config.set('Preprocessing Steps', 'show_registration_results', self.ui.cb_show_registration_results.isChecked())
        config.set('Preprocessing Steps', 'slicetime_correction', self.ui.cb_slicetime_correction.isChecked())
        config.set('Preprocessing Steps', 'spatial_filtering', self.ui.cb_spatial_filtering.isChecked())
        config.set('Preprocessing Steps', 'temporal_filtering', self.ui.cb_temporal_filtering.isChecked())
        config.set('Preprocessing Steps', 'write_logfile', self.ui.cb_write_logfile.isChecked())
        config.set('Preprocessing Steps', 'FWHM in mm', self.ui.sb_fwhm.value())
        config.set('Preprocessing Steps', 'High Pass Filter Cutoff in s', self.ui.sb_hp_cutoff.value())
        config.set('Preprocessing Steps', 'Low Pass Filter Cutoff in s', self.ui.sb_lp_cutoff.value())
        config.set('Preprocessing Steps', 'Maximum number of voxel', self.ui.sb_max_num_voxel.value())
        config.set('Preprocessing Steps', 'create_average_mask', self.ui.cb_create_average_mask.isChecked())
        config.set('Preprocessing Steps', 'create_mask_subject', self.ui.cb_create_mask_subject.isChecked())
        config.set('Preprocessing Steps', 'repetition_time', self.ui.dsb_repetition_time.value())
        config.set('Preprocessing Steps', 'output_directory', self.ui.le_directory.text())
        config.set('Preprocessing Steps', 'prefix', self.ui.le_prefix.text())

        config.add_section('Registration Settings')
        config.set('Registration Settings', 'Atlas Template', self.ui.comboBox_template.currentIndex())
        config.set('Registration Settings', 'ridid_registration', self.ui.cb_rigid_registration.isChecked())
        config.set('Registration Settings', 'max_iterations_p1', self.ui.sb_max_iterations_p1.value())
        config.set('Registration Settings', 'prealing_images', self.ui.cb_prealing_images.isChecked())
        config.set('Registration Settings', 'affine_registration', self.ui.cb_affine_registration.isChecked())
        config.set('Registration Settings', 'max_iterations_p2', self.ui.sb_max_iterations_p2.value())
        config.set('Registration Settings', 'deformable_registration', self.ui.cb_deformable_registration.isChecked())
        config.set('Registration Settings', 'max_iterations_p3', self.ui.sb_max_iterations_p3.value())
        config.set('Registration Settings', 'max_deformation in mm', self.ui.sb_max_deformation.value())
        config.set('Registration Settings', 'Interpolator function', self.ui.comboBox_interpol_func.currentIndex())
        config.set('Registration Settings', 'Output resolution', self.ui.le_output_resolution.text())

        with open('preprocessing.cfg', 'wb') as configfile:
            config.write(configfile)  
        
#...und laden der Einstellungen
    def load_settings(self):
        config = ConfigParser.RawConfigParser()
        config.read('preprocessing.cfg')
        self.ui.cb_atlas_registration.setChecked(config.getboolean('Preprocessing Steps', 'atlas_registration'))
        self.ui.cb_create_mask.setChecked(config.getboolean('Preprocessing Steps', 'create_mask'))
        self.ui.cb_debug_output.setChecked(config.getboolean('Preprocessing Steps', 'debug_output')
        self.ui.cb_fieldmap_correction.setChecked(config.getboolean('Preprocessing Steps', 'fieldmap_correction'))
        self.ui.cb_high_pass.setChecked(config.getboolean('Preprocessing Steps', 'high_pass'))
        self.ui.cb_low_pass.setChecked(config.getboolean('Preprocessing Steps', 'low_pass'))
        self.ui.cb_movement_correction.setChecked(config.getboolean('Preprocessing Steps', 'movement_correction'))
        self.ui.cb_set_repetition.setChecked(config.getboolean('Preprocessing Steps', 'set_repetition'))
        self.ui.cb_show_mask.setChecked(config.getboolean('Preprocessing Steps', 'show_mask'))
        self.ui.cb_show_registration_results.setChecked(config.getboolean('Preprocessing Steps', 'show_registration_results'))
        self.ui.cb_slicetime_correction.setChecked(config.getboolean('Preprocessing Steps', 'slicetime_correction'))
        self.ui.cb_spatial_filtering.setChecked(config.getboolean('Preprocessing Steps', 'spatial_filtering'))
        self.ui.cb_temporal_filtering.setChecked(config.getboolean('Preprocessing Steps', 'temporal_filtering'))
        self.ui.cb_write_logfile.setChecked(config.getboolean('Preprocessing Steps', 'write_logfile'))
        self.ui.sb_fwhm.setValue(config.getint('Preprocessing Steps', 'FWHM in mm'))
        self.ui.sb_hp_cutoff.setValue(config.getint('Preprocessing Steps', 'High Pass Filter Cutoff in s'))
        self.ui.sb_lp_cutoff.setValue(config.getint('Preprocessing Steps', 'Low Pass Filter Cutoff in s'))
        self.ui.sb_max_num_voxel.setValue(config.getint('Preprocessing Steps', 'Maximum number of voxel'))
        self.ui.cb_create_average_mask.setChecked(config.getboolean('Preprocessing Steps', 'create_average_mask'))
        self.ui.create_mask_subject.setChecked(config.getboolean('Preprocessing Steps', 'create_mask_subject'))
        self.ui.dsb_repetition_time.setValue(config.getfloat('Preprocessing Steps', 'repetition_time'))
        self.ui.le_directory.setText(config.get('Preprocessing Steps', 'output_directory'))
        self.ui.le_prefix.setText(config.get('Preprocessing Steps', 'prefix'))

        self.ui.comboBox_template.setCurrentIndex(config.getint('Registration Settings', 'Atlas Template'))
        self.ui.cb_rigid_registration.setChecked(config.getboolean('Registration Settings', 'ridid_registration'))
        self.ui.sb_max_iterations_p1.setValue(config.getint('Registration Settings', 'max_iterations_p1'))
        self.ui.cb_prealing_images.setChecked(config.getboolean('Registration Settings', 'prealing_images'))
        self.ui.cb_affine_registration.setChecked(config.getboolean('Registration Settings', 'affine_registration'))
        self.ui.sb_max_iterations_p2.setValue(config.getint('Registration Settings', 'max_iterations_p2'))
        self.ui.cb_deformable_registration.setChecked(config.getboolean('Registration Settings', 'deformable_registration'))
        self.ui.sb_max_iterations_p3.setValue(config.getint('Registration Settings', 'max_iterations_p3'))
        self.ui.sb_max_deformation.setValue(config.getint('Registration Settings', 'max_deformation in mm'))
        self.ui.comboBox_interpol_func.setCurrentIndex(config.getint('Registration Settings', 'Interpolator function'))
        self.ui.le_output_resolution.setText(config.get('Registration Settings', 'Output resolution'))

#help fehlt
    def show_help(self):
        if( self.ui.comboBox_help.currentText() != "" ):
            self.ui.textBrowser_help.setUrl(QtCore.QUrl(self.helpDict[str(self.ui.comboBox_help.currentText())]))

# Template auswahlcheck - wenn die nicht vorhanden sind (lipsia-paket), dann fehler schmeißen und wieder ausschalten
    def change_atlas_reg(self):
        if(self.ui.cb_atlas_registration.isChecked()):
            if(not self.ui.comboBox_template.count()):
                self.throwError("No template image was found. Please make sure you have installed the lipsia-sandbox package!")
                self.ui.check_atlas_registration.setChecked(False)





            
        

# Execute Function
if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    myapp = PreProc()
    myapp.show()
    sys.exit(app.exec_())
