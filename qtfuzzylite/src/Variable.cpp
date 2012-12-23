/*
 * InputVariable.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Variable.h"
#include "fl/qt/Term.h"
#include "fl/qt/Wizard.h"
#include "fl/qt/Viewer.h"


#include <fl/Headers.h>

#include <QtGui/QMessageBox>
#include <QtGui/QListWidgetItem>
#include <QtGui/QSplitter>

namespace fl {
    namespace qt {

        Variable::Variable(QWidget* parent, Qt::WindowFlags f)
        : QDialog(parent, f), _previouslySelected(NULL),
        ui(new Ui::Variable), viewer(new Viewer),
        variable(NULL) {
//            setWindowFlags(Qt::DI);
        }

        Variable::~Variable() {
            disconnect();
            delete ui;
        }

        void Variable::setup(VariableType type) {
            if (type == INPUT_VARIABLE)
                variable = new InputVariable("", 0, 1);
            else if (type == OUTPUT_VARIABLE)
                variable = new OutputVariable("", 0, 1);

            ui->setupUi(this);
            
            QList<int> sizes;
            sizes << .75 * size().width() << .25 * size().width() ;
            ui->splitter2->setSizes(sizes);
//            ui->splitter2->setStretchFactor(1,0);
            
            viewer->setup(variable);
            ui->splitter->addWidget(viewer);
            setWindowTitle("Add variable");

//            adjustSize();
//            QSize thisSize = size();
//            if (type != OUTPUT_VARIABLE) {
//                thisSize.setHeight(thisSize.height() - 85);
//            }

            ui->gbx_output->setVisible(type == OUTPUT_VARIABLE);

            QRect scr = parentWidget()->geometry();
            move(scr.center() - rect().center());
            connect();

        }

        void Variable::connect() {
            QObject::connect(ui->btn_add_term, SIGNAL(clicked()),
                    this, SLOT(onClickAddTerm()));
            QObject::connect(ui->btn_edit_term, SIGNAL(clicked()),
                    this, SLOT(onClickEditTerm()));
            QObject::connect(ui->btn_remove_term, SIGNAL(clicked()),
                    this, SLOT(onClickRemoveTerm()));

            QObject::connect(ui->btn_term_up, SIGNAL(clicked()),
                    this, SLOT(onClickMoveUp()));
            QObject::connect(ui->btn_term_down, SIGNAL(clicked()),
                    this, SLOT(onClickMoveDown()));

            QObject::connect(ui->lvw_terms, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onSelectTerm()));
            QObject::connect(ui->lvw_terms, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onSelectTerm()));
            QObject::connect(ui->lvw_terms, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                    this, SLOT(onDoubleClickTerm(QListWidgetItem*)));
            QObject::connect(ui->lvw_terms, SIGNAL(itemClicked(QListWidgetItem*)),
                    this, SLOT(onClickTerm(QListWidgetItem*)));

            QObject::connect(ui->sbx_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeMinRange(double)));
            QObject::connect(ui->sbx_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeMaxRange(double)));

            QObject::connect(ui->btn_wizard, SIGNAL(clicked()),
                    this, SLOT(onClickWizard()));

            QObject::connect(viewer, SIGNAL(valueChanged(double)),
                    this, SLOT(showSelectedTerms()), Qt::QueuedConnection);
        }

        void Variable::disconnect() {
            QObject::disconnect(ui->btn_add_term, SIGNAL(clicked()),
                    this, SLOT(onClickAddTerm()));
            QObject::disconnect(ui->btn_edit_term, SIGNAL(clicked()),
                    this, SLOT(onClickEditTerm()));
            QObject::disconnect(ui->btn_remove_term, SIGNAL(clicked()),
                    this, SLOT(onClickRemoveTerm()));

            QObject::disconnect(ui->btn_term_up, SIGNAL(clicked()),
                    this, SLOT(onClickMoveUp()));
            QObject::disconnect(ui->btn_term_down, SIGNAL(clicked()),
                    this, SLOT(onClickMoveDown()));

            QObject::disconnect(ui->lvw_terms, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onSelectTerm()));
            QObject::disconnect(ui->lvw_terms, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onSelectTerm()));
            QObject::disconnect(ui->lvw_terms, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                    this, SLOT(onDoubleClickTerm(QListWidgetItem*)));


            QObject::disconnect(ui->sbx_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeMinRange(double)));
            QObject::disconnect(ui->sbx_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeMaxRange(double)));

            QObject::disconnect(ui->btn_wizard, SIGNAL(clicked()),
                    this, SLOT(onClickWizard()));
        }

        void Variable::showEvent(QShowEvent* event) {
            //            ui->canvas->scene()->setSceneRect(ui->canvas->viewport()->rect());
            //            ui->canvas->fitInView(0, 0, ui->canvas->scene()->width(),
            //                    ui->canvas->scene()->height(), Qt::IgnoreAspectRatio);
            reloadModel();
            QWidget::showEvent(event);

        }

        void Variable::edit(const InputVariable* inputVariable) {
            variable->setMinimum(inputVariable->getMinimum());
            variable->setMaximum(inputVariable->getMaximum());
            for (int i = 0; i < inputVariable->numberOfTerms(); ++i) {
                this->variable->addTerm(inputVariable->getTerm(i)->copy());
            }

            setWindowTitle("Edit variable");
            ui->led_name->setText(QString::fromStdString(inputVariable->getName()));
            reloadModel();
        }

        void Variable::edit(const OutputVariable* outputVariable) {
            variable->setMinimum(outputVariable->getMinimum());
            variable->setMaximum(outputVariable->getMaximum());
            for (int i = 0; i < outputVariable->numberOfTerms(); ++i) {
                this->variable->addTerm(outputVariable->getTerm(i)->copy());
            }

            setWindowTitle("Edit variable");
            ui->led_name->setText(QString::fromStdString(outputVariable->getName()));

            OutputVariable* editable = dynamic_cast<OutputVariable*> (this->variable);
            editable->setDefaultValue(outputVariable->getDefaultValue());
            editable->setDefuzzifier(outputVariable->getDefuzzifier());

            editable->setDefuzzifiedValue(outputVariable->getDefuzzifiedValue());
            editable->setLockDefuzzifiedValue(outputVariable->lockDefuzzifiedValue());

            reloadModel();
        }

        void Variable::redraw() {
            viewer->refresh();
            showSelectedTerms();
        }

        void Variable::showSelectedTerms() {
            ui->ptx_terms->clear();
            bool empty = true;
            for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    empty = false;
                    viewer->draw(variable->getTerm(i));
                    ui->ptx_terms->appendPlainText(QString::fromStdString(
                            variable->getTerm(i)->toString()));
                }
            }
            if (empty) ui->ptx_terms->appendPlainText("No terms selected");
        }

        /**
         * Button actions...
         */

        void Variable::accept() {
            OutputVariable* outputVariable = dynamic_cast<OutputVariable*> (variable);
            if (outputVariable) {
                try {
                    outputVariable->setDefaultValue(
                            fl::Op::Scalar(ui->led_default->text().toStdString()));
                } catch (fl::Exception& ex) {
                    std::ostringstream message;
                    message << ex.what() << std::endl <<
                            "Expected numeric values (including nan or inf)" << std::endl;
                    QMessageBox::critical(this, tr("Error"),
                            QString::fromStdString(message.str()),
                            QMessageBox::Ok);
                    return;
                }
                outputVariable->setLockDefuzzifiedValue(ui->chx_lock->isChecked());
            }
            variable->setName(ui->led_name->text().toStdString());
            QDialog::accept();
        }

        void Variable::reject() {
            FL_LOG("Deleting variable " << variable->toString());
            delete variable;
            variable = NULL;
            QDialog::reject();
        }

        void Variable::onChangeMinRange(double) {
            if (fl::Op::IsGE(ui->sbx_min->value(), ui->sbx_max->value())) {
                ui->sbx_max->setValue(ui->sbx_min->value() + .1);
            }
            variable->setMinimum(ui->sbx_min->value());
            variable->setMaximum(ui->sbx_max->value());

            redraw();
        }

        void Variable::onChangeMaxRange(double) {
            if (fl::Op::IsLE(ui->sbx_max->value(), ui->sbx_min->value())) {
                ui->sbx_min->setValue(ui->sbx_max->value() - .1);
            }
            variable->setMinimum(ui->sbx_min->value());
            variable->setMaximum(ui->sbx_max->value());

            redraw();
        }

        void Variable::onClickWizard() {
            Wizard* window = new Wizard(this);
            window->setup(ui->led_name->text().toStdString());
            if (window->exec()) {
                for (std::size_t i = 0; i < window->terms.size(); ++i) {
                    variable->addTerm(window->terms[i]);
                }
                reloadModel();
            }
        }

        void Variable::onClickAddTerm() {
            Term* window = new Term(this);
            window->setup(variable);
            if (window->exec()) {
                variable->addTerm(window->copySelectedTerm());
                reloadModel();
                ui->lvw_terms->setFocus();
                ui->lvw_terms->item(ui->lvw_terms->count() - 1)->setSelected(true);
            }
            delete window;
        }

        void Variable::onClickRemoveTerm() {
            std::ostringstream message;
            message << "Do you want to delete the following terms?"
                    << std::endl;

            for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    message << "<" << variable->getTerm(i)->getName() << ">: "
                            << variable->getTerm(i)->toString() << std::endl;
                }
            }

            QMessageBox::StandardButton clicked = QMessageBox::warning(this, "Delete term",
                    QString::fromStdString(message.str()),
                    QMessageBox::Yes | QMessageBox::No);
            if (clicked == QMessageBox::Yes) {
                for (int i = ui->lvw_terms->count() - 1; i >= 0; --i) {
                    if (ui->lvw_terms->item(i)->isSelected()) {
                        delete (variable->removeTerm(i));
                    }
                }
                reloadModel();
            }

        }

        void Variable::onClickEditTerm() {
            if (ui->lvw_terms->selectedItems().size() > 1) {
                std::ostringstream message;
                message << "Do you want to edit the following terms?"
                        << std::endl;
                for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                    if (ui->lvw_terms->item(i)->isSelected()) {
                        message << "<" << variable->getTerm(i)->getName() << ">: "
                                << variable->getTerm(i)->toString() << std::endl;
                    }
                }
                QMessageBox::StandardButton clicked = QMessageBox::warning(this,
                        "Edit multiple terms",
                        QString::fromStdString(message.str()),
                        QMessageBox::Yes | QMessageBox::No);
                if (clicked == QMessageBox::No) {
                    return;
                }
            }

            std::vector<int> selected;
            for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    selected.push_back(i);
                    Term* window = new Term(this);
                    window->setup(variable);
                    window->edit(variable->getTerm(i));
                    if (window->exec()) {
                        fl::Term* term = window->copySelectedTerm();
                        delete variable->removeTerm(i);
                        variable->insertTerm(term, i);
                    }
                    delete window;
                }
            }
            reloadModel();
            for (std::size_t i = 0; i < selected.size(); ++i) {
                ui->lvw_terms->item(selected[i])->setSelected(true);
            }
        }

        void Variable::onSelectTerm() {
            ui->btn_edit_term->setEnabled(
                    ui->lvw_terms->selectedItems().size() > 0);
            ui->btn_remove_term->setEnabled(
                    ui->lvw_terms->selectedItems().size() > 0);
            ui->btn_term_down->setEnabled(ui->lvw_terms->selectedItems().size() > 0);
            ui->btn_term_up->setEnabled(ui->lvw_terms->selectedItems().size() > 0);
            redraw();
        }

        void Variable::onDoubleClickTerm(QListWidgetItem* item) {
            if (item) {
                onClickEditTerm();
            }
        }

        void Variable::onClickTerm(QListWidgetItem* item) {
            if (item == _previouslySelected) {
                _previouslySelected = NULL;
                item->setSelected(false);
            } else {
                _previouslySelected = item;
            }
        }

        void Variable::onClickMoveUp() {
            std::vector<int> newPositions;
            bool rotate = false;
            for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    int newPosition = i - 1 + rotate;
                    if (newPosition < 0) {
                        newPosition = ui->lvw_terms->count() - 1;
                        newPositions.push_back(newPosition);
                        rotate = true;
                    } else
                        newPositions.push_back(newPosition - rotate);

                    fl::Term* term = variable->removeTerm(i);
                    variable->insertTerm(term, newPosition);
                }
            }
            reloadModel();
            for (std::size_t i = 0; i < newPositions.size(); ++i) {
                ui->lvw_terms->item(newPositions[i])->setSelected(true);
            }
        }

        void Variable::onClickMoveDown() {
            std::vector<int> newPositions;
            bool rotate = false;
            for (int i = ui->lvw_terms->count() - 1; i >= 0; --i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    int newPosition = i + 1 - rotate;
                    if (newPosition >= ui->lvw_terms->count()) {
                        newPosition = 0;
                        newPositions.push_back(newPosition);
                        rotate = true;
                    } else
                        newPositions.push_back(newPosition + rotate);
                    fl::Term* term = variable->removeTerm(i);
                    variable->insertTerm(term, newPosition);
                }
            }
            reloadModel();
            for (std::size_t i = 0; i < newPositions.size(); ++i) {
                ui->lvw_terms->item(newPositions[i])->setSelected(true);
            }
        }

        void Variable::reloadModel() {
            ui->lvw_terms->clear();
            for (int i = 0; i < variable->numberOfTerms(); ++i) {
                ui->lvw_terms->addItem(QString::fromStdString(variable->getTerm(i)->getName()));
            }

            OutputVariable* outputVariable = dynamic_cast<OutputVariable*> (variable);
            if (outputVariable) {
                ui->led_default->setText(QString::number(outputVariable->getDefaultValue()));
                ui->chx_lock->setChecked(outputVariable->lockDefuzzifiedValue());
            }

            ui->sbx_min->setValue(variable->getMinimum());
            ui->sbx_max->setValue(variable->getMaximum());

            redraw();

        }

    }
}
