#include "VegaMain.h"
#include "Tournament.h"


RankRatedGroup::RankRatedGroup()
{   int i;
	String str;
	CtrlLayout(*this, "");

    for (i=0; i<GROUPS; i++) {
    	Add(editgroup[i][0]);
    	editgroup[i][0].LeftPosZ(104, 48).TopPosZ(72+i*40, 19);
		editgroup[i][0] <<= limitgroup[i][0] = 0;

    	Add(editgroup[i][1]);
    	editgroup[i][1].LeftPosZ(216, 48).TopPosZ(72+i*40, 19);
		editgroup[i][1] <<= limitgroup[i][1] = 0;
	}
    btnClose <<= THISBACK(CloseCB);
    btnDone <<= THISBACK(DoneCB);
}


void RankRatedGroup::DoneCB()
{ int g;
  for (g=0; g<GROUPS; g++) // loop on groups to grab the limits
      { limitgroup[g][0] =  StrIntValue( AsString(~editgroup[g][0]) );
        limitgroup[g][1] =  StrIntValue( AsString(~editgroup[g][1]) );
      }
  SaveRankRatedGroup();
  Close();
}

void RankRatedGroup::CloseCB()
{
	Close();
}

void RankRatedGroup::SaveRankRatedGroup()
{ int i, k, j, g, countAvailable;
  int arr[N_PLAYER_MAX+1];
  String stringa, strname, str;
  extern int  CompareScoreName(const void *pid1, const void *pid2);
  FileOut save( TD.PATHDIR + "rankgrp.txt");

  save << TD.NAMETORNEO << "\n";
  save << TD.PLACETORNEO << " - " <<  TD.DATATORNEO_B <<", "<<TD.DATATORNEO_E  << "\n\n";
  save << t_("Rated Player Group at round ") << TD.currentRound<< "\n";

  for (g=0; g<GROUPS; g++) // loop on groups
  { // grab the players in the group g
    countAvailable = 0;
    for (i=1; i<=TD.NPlayer; i++)
      { if (player[i].RATING>limitgroup[g][0] && player[i].RATING<=limitgroup[g][1])          // the player's score should be in the limits
           { arr[countAvailable] = i;       // set the array to be sorted
             countAvailable++;
           }
      }
    if (countAvailable) {
       qsort(arr, countAvailable, sizeof(int), CompareScoreName);

       save << "\n\nGroup limits:  " << limitgroup[g][0] << " < rating <= "<< limitgroup[g][1] << "\n";
       save<<"Pos score ID NAME                 | Rtg  Tit Fed | ";
       j=1;
       while (TD.tyebreakorder[j]) {
          switch ( TD.tyebreakorder[j] )
          { case 1: save << "  " + TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 2: save << "  " + TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 3: save << "  " + TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 4: save << "  " + TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 5: save << "  " + TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 6: save << "  " + TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 7: save << "  " + TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 8: save << "  " + TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 9: save << "  " + TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
          }
          j++;
       }
       save<< "\n";
       save<<"------------------------------------------------------------";
       j=1;
       while (TD.tyebreakorder[j]) {
          save<<"--------";
          j++;
       }
       save<<"\n";

       for (k=0; k<countAvailable; k++)
        { i=arr[k];
	      if (player[i].isAvailable != "0") strname = NFormat("%-20.20s", player[i].name);
          else strname = NFormat("%-17.17s(W)", player[i].name);
          save << NFormat("%3d %4.1f %3d %s | %4d %3s %3s | ",
	         k+1, //position
             player[i].tiebreak[SCORE],
             i,
	         strname,
	         player[i].RATING,
	         player[i].codetitle,
	         player[i].country);
          j=1;
          while (TD.tyebreakorder[j]) {
             switch ( TD.tyebreakorder[j] )
             { case 1: save << NFormat( " %6.2f ", player[i].tiebreak[BUC_CUT1]); break;
               case 2: save << NFormat( " %6.2f ", player[i].tiebreak[BUC_TOT]); break;
               case 3: save << NFormat( " %6.2f ", player[i].tiebreak[BUC_MED]); break;
               case 4: save << NFormat( " %6.2f ", player[i].tiebreak[SONN_BERG]); break;
               case 5: save << NFormat( " %6.2f ", player[i].tiebreak[CUMUL]); break;
               case 6: save << NFormat( " %6.2f ", player[i].tiebreak[ARO]); break;
               case 7: save << NFormat( " %4.0f ", player[i].tiebreak[MOST_BLACKS]); break;
               case 8: save << NFormat( " %4.0f ", player[i].tiebreak[MOST_WINS]); break;
               case 9: save << NFormat( " %4.0f ", player[i].tiebreak[APRO]); break;
             }
             j++;
          }
          save << "\n";
        }
    }
  }
  save<<"\n Generated by Vega - www.vegachess.com\n";
  save.Close();
}

///////////////////// rank Date group ///////////////////////////////

RankDateGroup::RankDateGroup()
{
    int i;
	String str;
	CtrlLayout(*this, "");

    for (i=0; i<GROUPS; i++) {
    	Add(editgroup[i][0]);
    	editgroup[i][0].LeftPosZ(104, 48).TopPosZ(72+i*40, 19);
		editgroup[i][0] <<= limitgroup[i][0] = 0;

    	Add(editgroup[i][1]);
    	editgroup[i][1].LeftPosZ(216, 48).TopPosZ(72+i*40, 19);
		editgroup[i][1] <<= limitgroup[i][1] = 0;
	}
    btnClose <<= THISBACK(CloseCB);
    btnDone <<= THISBACK(DoneCB);
}

void RankDateGroup::CloseCB()
{
	Close();
}

void RankDateGroup::DoneCB()
{ int g;
  for (g=0; g<GROUPS; g++) // loop on groups to grab the limits
      { limitgroup[g][0] =  StrIntValue( AsString(~editgroup[g][0]) );
        limitgroup[g][1] =  StrIntValue( AsString(~editgroup[g][1]) );
      }
  SaveRankDateGroup();
  Close();
}

void RankDateGroup::SaveRankDateGroup()
{ int i, k, j, g, countAvailable;
  int arr[N_PLAYER_MAX+1];
  String stringa, strname, str;
  extern int cmpdata(int d1, int d2);
  extern int CompareScoreName(const void *pid1, const void *pid2);
  FileOut save( TD.PATHDIR + "rankdagr.txt");

  save<<  TD.NAMETORNEO << "\n";
  save<< TD.PLACETORNEO << " - " <<  TD.DATATORNEO_B <<", "<<TD.DATATORNEO_E  << "\n\n";
  save<< t_("Rated Player Group at round ")<< TD.currentRound<< "\n";

  for (g=0; g<GROUPS; g++) // loop on groups
  { // grab the players in the group g
    countAvailable = 0;
    for (i=1; i<=TD.NPlayer; i++)
      { if ( cmpdata( StrIntValue(player[i].data), limitgroup[g][0])==1
             && cmpdata( StrIntValue(player[i].data),limitgroup[g][1])<=0)  // the player's score should be in the limits
           { arr[countAvailable] = i;   // set the array to be sorted
             countAvailable++;
           }
      }
    if (countAvailable) {
       qsort(arr, countAvailable, sizeof(int), CompareScoreName);

       save << "\n\n" << "Group limits:  " << limitgroup[g][0] << " < date <= "<< limitgroup[g][1] << "\n";
       save<<"Pos score ID NAME                 | Rtg  Tit Fed |  date  | ";
       j=1;
      while (TD.tyebreakorder[j]) {
          switch ( TD.tyebreakorder[j] )
          { case 1: save << TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 2: save << TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 3: save << TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 4: save << TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 5: save << TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 6: save << TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 7: save << TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 8: save << TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
            case 9: save << TD.tyebreakname[ TD.tyebreakorder[j] ]; break;
          }
          j++;
       }
       save<< "\n";
       save<<"------------------------------------------------------------";
       j=1;
       while (TD.tyebreakorder[j]) {
          save<<"--------";
          j++;
       }
       save<<"\n";
       for (k=0; k<countAvailable; k++)
        { i=arr[k];
	      if (player[i].isAvailable != "0") strname = NFormat("%-20.20s", player[i].name);
          else strname = NFormat("%-17.17s(W)", player[i].name);
          save << NFormat("%3d %4.1f %3d %s | %4d %3s %3s | ",
	         k+1, //position
             player[i].tiebreak[SCORE],
             i,
	         strname,
	         player[i].RATING,
	         player[i].codetitle,
	         player[i].country);
          j=1;
          while (TD.tyebreakorder[j]) {
             switch ( TD.tyebreakorder[j] )
             { case 1: save << NFormat( " %6.2f ", player[i].tiebreak[BUC_CUT1]); break;
               case 2: save << NFormat( " %6.2f ", player[i].tiebreak[BUC_TOT]); break;
               case 3: save << NFormat( " %6.2f ", player[i].tiebreak[BUC_MED]); break;
               case 4: save << NFormat( " %6.2f ", player[i].tiebreak[SONN_BERG]); break;
               case 5: save << NFormat( " %6.2f ", player[i].tiebreak[CUMUL]); break;
               case 6: save << NFormat( " %6.2f ", player[i].tiebreak[ARO]); break;
               case 7: save << NFormat( " %4.0f ", player[i].tiebreak[MOST_BLACKS]); break;
               case 8: save << NFormat( " %4.0f ", player[i].tiebreak[MOST_WINS]); break;
               case 9: save << NFormat( " %4.0f ", player[i].tiebreak[APRO]); break;
             }
             j++;
          }
          save << "\n";
        }
    }
  }
  save<<"\n Generated by Vega - www.vegachess.com\n";
  save.Close();
}
