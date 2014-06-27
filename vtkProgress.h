#ifndef VTKPROGRESS_H
#define VTKPROGRESS_H


#include <vtkCommand.h>
#include <vtkAlgorithm.h>
#include <QtGui>

class vtkProgress : public QObject,public vtkCommand
{
	Q_OBJECT

public:
	vtkProgress()
	{ 
		m_nProgress=0;
		iStage=0;
		nStage=-1;
	}

	virtual ~vtkProgress(){ }
	int m_nProgress;
	//int m_nStage;

	int iStage, nStage;
	QString text;

signals:
     void ProgressUpdate(int);
	 void TextUpdate(QString);


public:
	static vtkProgress *New() 
	{ 
		return new vtkProgress;
	}
	//void Delete()
	//{
	//	//delete this; 
	//}

	void Update()
	{
		emit TextUpdate(text);
		emit ProgressUpdate(m_nProgress);
	}

	virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
		vtkAlgorithm *process=reinterpret_cast<vtkAlgorithm*>(caller);

		if (nStage == -1)
		{
			m_nProgress = (int)(process->GetProgress()*100);
		} 
		else
		{
			m_nProgress = (int)(process->GetProgress()*100/(double)nStage + 100*iStage/(double)nStage);
		}

		//switch(m_nStage)
		//{
		//	case 0:
		//		{
		//			m_nProgress = (int) (process->GetProgress()*100);
		//		}
		//		break;
		//	case 1:
		//		{
		//			m_nProgress = (int) (process->GetProgress()*20);
		//		}
		//		break;
		//	case 2:
		//		{
		//			m_nProgress = (int) (process->GetProgress()*30)+20;
		//		}
		//		break;
		//	case 3:
		//		{
		//			m_nProgress = (int) (process->GetProgress()*50)+50;
		//		}
		//		break;
		//	default:
		//		break;
		//}

		emit TextUpdate(text);
		emit ProgressUpdate(m_nProgress);
	}
};

#endif