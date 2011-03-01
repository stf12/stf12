/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : menu.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : This file includes the menu navigation driver for the
*                      STM3210B-EVAL demonstration.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
typedef void (* tMenuFunc)(void);
typedef struct sMenuItem * tMenuItem;
typedef struct sMenu * tMenu;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t MenuItemIndex = 0, nMenuLevel = 0;
uint8_t ItemNumb[MAX_MENU_LEVELS];

tMenuItem psMenuItem, psCurrentMenuItem;
tMenu psPrevMenu[MAX_MENU_LEVELS];
tMenu psCurrentMenu;

struct sMenuItem
{
  uint8_t* pszTitle;
  tMenuFunc pfMenuFunc;
  tMenuFunc pfUpDownMenuFunc;
  tMenu psSubMenu;
};

struct sMenu
{
  uint8_t* pszTitle;
  tMenuItem psItems;
  uint8_t nItems;
};

const uint32_t IconsAddr[8] = {0x00622200, 0x00624D00, 0x00627800, 0x0062A300, 0x0062CE00,
                     0x0062F900, 0x00632400, 0x00634F00};


const uint32_t SlideAddr[14] = {0x00414400, 0x00439D00, 0x0045F600, 0x00484F00, 0x004AA800,
                      0x004D0100, 0x004F5A00, 0x0051B300, 0x00540C00, 0x00566500,
                      0x0058BE00, 0x005B1700, 0x005D7000, 0x005FC900};

const uint32_t SlidesSpeech[14] = {0x004142A2, 0x003D0147, 0x00373C6A, 0x00360218, 0x0031C734,
                         0x0029C386, 0x00251239, 0x001F9F2A, 0x0019AF3A, 0x00174BD4,
                         0x00115BE8, 0x000DF1D8, 0x0007ABCE, 0x0003C091};

const uint32_t DelayTiming[14] = {2530, 3430, 730, 2515, 4765, 2790, 3240, 3530, 1420, 3530,
                        2030, 3730, 2330, 2130};

uint8_t SlidesCheck[6] = {0x42, 0x4D, 0x42, 0x58, 0x02, 0x00};
uint8_t Icons64Check[6] = {0x42, 0x4D, 0x42, 0x2A, 0x00, 0x00};
uint8_t Icons128Check[6] = {0x42, 0x4D, 0x42, 0x80, 0x00, 0x00};

/*------------------------------ Menu level 4 -------------------------------*/

/*------------------------------ Menu level 3 -------------------------------*/

struct sMenuItem STOPMenuItems[] = {{"    Exit: EXTI      ", EnterSTOPMode_EXTI, IdleFunc},
                                    {"  Exit: RTC Alarm   ", EnterSTOPMode_RTCAlarm, IdleFunc},
                                    {"       Return       ", ReturnFunc, IdleFunc}};
struct sMenu STOPMenu = {"     STOP Mode      ", STOPMenuItems, countof(STOPMenuItems)};

struct sMenuItem STANDBYMenuItems[] = {{"  Exit: Wakeup Pin  ", EnterSTANDBYMode_WAKEUP, IdleFunc},
                                       {"  Exit: RTC Alarm   ", EnterSTANDBYMode_RTCAlarm, IdleFunc},
                                       {"       Return       ", ReturnFunc, IdleFunc}};
struct sMenu STANDBYMenu = {"   STANDBY Mode     ", STANDBYMenuItems, countof(STANDBYMenuItems)};

struct sMenuItem SettingTimeMenuItems[] = { {"       Adjust       ", Time_Adjust, IdleFunc},
                                            {"        Show        ", Time_Show, IdleFunc},
                                            {"       Return       ", ReturnFunc, IdleFunc}};
struct sMenu TimeMenu = {"        Time        ", SettingTimeMenuItems, countof(SettingTimeMenuItems)};

struct sMenuItem SettingDateMenuItems[] = { {"       Adjust       ", Date_Adjust, IdleFunc},
                                            {"        Show        ", Date_Show, IdleFunc},
                                            {"       Return       ", ReturnFunc, IdleFunc}};
struct sMenu DateMenu = {"        Date        ", SettingDateMenuItems, countof(SettingDateMenuItems)};

struct sMenuItem SettingAlarmMenuItems[] = { {"       Adjust       ", Alarm_Adjust, IdleFunc},
                                             {"        Show        ", Alarm_Show, IdleFunc},
                                             {"       Return       ", ReturnFunc, IdleFunc}};
struct sMenu AlarmMenu = {"        Alarm       ", SettingAlarmMenuItems, countof(SettingAlarmMenuItems)};


/*------------------------------ Menu level 2 -------------------------------*/
struct sMenuItem AboutMenuItems[] = {{"       About        ", AboutFunc, IdleFunc},
                                     {"       Return       ", ReturnFunc, IdleFunc}};
struct sMenu AboutMenu = {"        About       ", AboutMenuItems, countof(AboutMenuItems)};

struct sMenuItem HelpMenuItems[] = {{"       Start        ", HelpFunc, IdleFunc},
                                    {"       Return       ", ReturnFunc, IdleFunc}};
struct sMenu HelpMenu = {"        Help        ", HelpMenuItems, countof(HelpMenuItems)};

struct sMenuItem FunnyMenuItems[] = {{"       Start        ", FunnyCircles, IdleFunc},
                                     {"       Return       ", ReturnFunc, IdleFunc}};
struct sMenu FunnyMenu = {"        Funny       ", FunnyMenuItems, countof(FunnyMenuItems)};

struct sMenuItem USBMA3SMenuItems[] = {{"       Start        ", Mass_Storage_Start, IdleFunc},
                                       {"       Return       ", ReturnFunc, IdleFunc}};
struct sMenu USBMA3SMenu = {"  USB Mass Storage  ", USBMA3SMenuItems, countof(USBMA3SMenuItems)};

struct sMenuItem TempSensorMenuItems[] = {{"    Temperature     ", Thermometer_Temperature, IdleFunc},
                                          {"       Return       ", ReturnFunc, IdleFunc}};
struct sMenu TempSensorMenu = {"    Thermometer     ", TempSensorMenuItems, countof(TempSensorMenuItems)};


struct sMenuItem LowPowerMenuItems[] = {{"        STOP        ", IdleFunc, IdleFunc, &STOPMenu},
                                        {"       STANDBY      ", IdleFunc, IdleFunc, &STANDBYMenu},
                                        {"       Return       ", ReturnFunc, IdleFunc}};
struct sMenu LowPowerMenu = {"    Low Power Mode  ", LowPowerMenuItems, countof(LowPowerMenuItems)};

struct sMenuItem CalendarMenuItems[] = {{"        Time        ", IdleFunc, IdleFunc, &TimeMenu},
                                        {"        Date        ", IdleFunc, IdleFunc, &DateMenu},
                                        {"        Alarm       ", IdleFunc, IdleFunc, &AlarmMenu},
                                        {"        Return      ", ReturnFunc, IdleFunc}};
struct sMenu CalendarMenu = {"       Calendar     ", CalendarMenuItems, countof(CalendarMenuItems)};

struct sMenuItem ProdPresMenuItems[] = {{"       Start        ", ProductPres, IdleFunc},
                                        {"       Return       ", ReturnFunc, IdleFunc}};
struct sMenu ProdPresMenu = {"Product Presentation", ProdPresMenuItems, countof(ProdPresMenuItems)};

/*------------------------------ Menu level 1 -------------------------------*/
struct sMenuItem MainMenuItems[] = {
  {"Product Presentation", IdleFunc, IdleFunc, &ProdPresMenu},
  {"       Calendar     ", IdleFunc, IdleFunc, &CalendarMenu},
  {"    Low Power Mode  ", IdleFunc, IdleFunc, &LowPowerMenu},
  {"    Thermometer     ", IdleFunc, IdleFunc, &TempSensorMenu},
  {"  USB Mass Storage  ", IdleFunc, IdleFunc, &USBMA3SMenu},
  {"        Funny       ", IdleFunc, IdleFunc, &FunnyMenu},
  {"        Help        ", IdleFunc, IdleFunc, &HelpMenu},
  {"        About       ", IdleFunc, IdleFunc, &AboutMenu}};
struct sMenu MainMenu = {"     Main menu      ", MainMenuItems, countof(MainMenuItems)};

/* Private function prototypes -----------------------------------------------*/
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Menu_Init
* Description    : Initializes the navigation menu.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Menu_Init(void)
{
  psCurrentMenu = &MainMenu;
  psPrevMenu[nMenuLevel] = psCurrentMenu;
  psMenuItem = MainMenuItems;
}

/*******************************************************************************
* Function Name  : DisplayMenu
* Description    : Displays the current menu.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DisplayMenu(void)
{
  uint32_t Line = 0, index = 0;
  tMenuItem psMenuItem2;

  /* Set the Back Color */
  LCD_SetBackColor(Blue);
  /* Set the Text Color */
  LCD_SetTextColor(White);

  /* Clear the LCD Screen */
  LCD_Clear(White);

  LCD_DisplayStringLine(Line, psCurrentMenu->pszTitle);
  Line += 24;

  /* Set the Back Color */
  LCD_SetBackColor(White);

  /* Set the Text Color */
  LCD_SetTextColor(Blue);

  while(!(index >= (psCurrentMenu->nItems)))
  {
    psMenuItem2 = &(psCurrentMenu->psItems[index]);
    LCD_DisplayStringLine(Line, psMenuItem2->pszTitle);
    index++;
    Line += 24;
  }
  /* Set the Back Color */
  LCD_SetBackColor(Green);

  /* Set the Text Color */
  LCD_SetTextColor(White);

  /* Get the current menu */
  psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);

  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
}

/*******************************************************************************
* Function Name  : SelFunc
* Description    : This function is executed when "SEL" push-buttton is pressed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SelFunc(void)
{
  psCurrentMenuItem = psMenuItem;

  if(psMenuItem->psSubMenu != '\0')
  {
    /* Update the current Item by the submenu */
    MenuItemIndex = 0;
    psCurrentMenu = psMenuItem->psSubMenu;
    psMenuItem = &(psCurrentMenu->psItems)[MenuItemIndex];
    DisplayMenu();
    nMenuLevel++;
    psPrevMenu[nMenuLevel] = psCurrentMenu;
  } 
  psCurrentMenuItem->pfMenuFunc();
}

/*******************************************************************************
* Function Name  : UpFunc
* Description    : This function is executed when any of "UP" push-butttons
*                  is pressed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UpFunc(void)
{
  /* Set the Back Color */
  LCD_SetBackColor(White);
  /* Set the Text Color */
  LCD_SetTextColor(Blue);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);

  if(MenuItemIndex > 0)
  {
    MenuItemIndex--;
  }
  else
  {
    MenuItemIndex = psCurrentMenu->nItems - 1;
  }
  /* Set the Back Color */
  LCD_SetBackColor(Green);
  /* Set the Text Color */
  LCD_SetTextColor(White);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
  ItemNumb[nMenuLevel] = MenuItemIndex;
}

/*******************************************************************************
* Function Name  : DownFunc
* Description    : This function is executed when any of "Down" push-butttons
*                  is pressed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DownFunc(void)
{
  /* Set the Back Color */
  LCD_SetBackColor(White);
  /* Set the Text Color */
  LCD_SetTextColor(Blue);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
      
  /* Test on the MenuItemIndex value before incrementing it */
  if(MenuItemIndex >= ((psCurrentMenu->nItems)-1))
  {
    MenuItemIndex = 0;
  }
  else
  {
    MenuItemIndex++;
  }
  /* Set the Back Color */
  LCD_SetBackColor(Green);
  /* Set the Text Color */
  LCD_SetTextColor(White);
  /* Get the current menu */
  psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
  LCD_DisplayStringLine(((MenuItemIndex + 1) * 24), psMenuItem->pszTitle);
  ItemNumb[nMenuLevel] = MenuItemIndex;
}

/*******************************************************************************
* Function Name  : ReturnFunc
* Description    : This function is executed when the "RETURN" menu is selected.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ReturnFunc(void)
{
  psMenuItem->pfUpDownMenuFunc();

  if(nMenuLevel == 0)
  {
    nMenuLevel++;
  }

  psCurrentMenu = psPrevMenu[nMenuLevel-1];
  psMenuItem = &psCurrentMenu->psItems[0];
  ItemNumb[nMenuLevel] = 0;
  MenuItemIndex = 0;
  nMenuLevel--;
  if(nMenuLevel != 0)
  {
    DisplayMenu();
  }
  else
  {
    ShowMenuIcons();
  }
}

/*******************************************************************************
* Function Name  : ReadKey
* Description    : Reads key from demoboard.
* Input          : None
* Output         : None
* Return         : Return RIGHT, LEFT, SEL, UP, DOWN or NOKEY
*******************************************************************************/
uint8_t ReadKey(void)
{
  /* "right" key is pressed */
  if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0))
  {
    while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) == Bit_RESET);
    return RIGHT; 
  }
  /* "left" key is pressed */
  if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1))
  {
    while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1) == Bit_RESET);
    return LEFT; 
  }
  /* "up" key is pressed */
  if(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8))
  {
    while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == Bit_RESET);
    return UP; 
  }
  /* "down" key is pressed */
  if(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14))
  {
    while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14) == Bit_RESET);
    return DOWN; 
  }
  /* "sel" key is pressed */
  if(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12))
  {
    while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) == Bit_RESET);
    return SEL; 
  }
  /* No key is pressed */
  else 
  {
    return NOKEY;
  }
}

/*******************************************************************************
* Function Name  : IdleFunc
* Description    : Idle function.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void IdleFunc(void)
{
  /* Nothing to execute: return */
  return;
}

/*******************************************************************************
* Function Name  : DisplayIcons
* Description    : Display menu icons.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DisplayIcons(void)
{
  uint32_t i = 0, j = 0, l = 0,  iconline = 0, iconcolumn = 0;
  
  iconline = 98;
  iconcolumn = 286;
    
  for(i = 0; i < 3; i++)
  {
    for(j = 0; j < 3; j++)
    {
      LCD_SetDisplayWindow(iconline, iconcolumn, 64, 84);
      LCD_DrawBMP(IconsAddr[l]);
      iconcolumn -= 85;
      l++;
      if(l == 8)
      {
        return;
      }
    }
    iconline += 65;
    iconcolumn = 286;
  }
}

/*******************************************************************************
* Function Name  : ShowMenuIcons
* Description    : Show the main menu icon.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ShowMenuIcons(void)
{
  uint32_t MyKey = 0, i = 0;  
  uint16_t IconRect[8][2] ={{34, 286}, {34, 201}, {34, 116}, 
                       {99, 286}, {99, 201}, {99, 116},
                       {164, 286}, {164, 201}};

  /* Disable the JoyStick interrupts */
  IntExtOnOffConfig(DISABLE);

  /* Initializes the Menu state machine */
  Menu_Init();

  MenuItemIndex = 0;
  
  /* Clear*/
  LCD_Clear(White);

  /* Set the Back Color */
  LCD_SetBackColor(Blue);

  /* Set the Text Color */
  LCD_SetTextColor(White);

  LCD_DisplayStringLine(Line0, psMenuItem->pszTitle);
  
  /* Set the Back Color */
  LCD_SetBackColor(White);

  /* Set the Text Color */
  LCD_SetTextColor(Blue);
  
  /* Displays Icons */    
  DisplayIcons();

  /* Disable LCD Window mode */
  LCD_WindowModeDisable(); 

  LCD_DrawRect(IconRect[0][0], IconRect[0][1], 65, 85);

  /* Endless loop */
  while(1)
  {
    /* Check which key is pressed */
    MyKey = ReadKey();

    /* If "UP" pushbutton is pressed */
    if(MyKey == UP)
    {
      /* Set the Text Color */
      LCD_SetTextColor(White);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 65, 85);
      if(i <= 2)
      {
        i += 6;
        if(i == 8)
        {
          i = 7;
        }
      }
      else
      {
        i -= 3;
      }
      /* Set the Text Color */
      LCD_SetTextColor(Blue);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 65, 85);

      /* Set the Back Color */
      LCD_SetBackColor(Blue);
      /* Set the Text Color */
      LCD_SetTextColor(White);
      /* Test on the MenuItemIndex value before incrementing it */
      if(MenuItemIndex <= 2)
      {
        MenuItemIndex += 6;
        if(MenuItemIndex == 8)
        {
          MenuItemIndex = 7;
        }
      }
      else
      {
        MenuItemIndex -= 3;
      }
      /* Get the current menu */
      psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
      LCD_DisplayStringLine(Line0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
    }
    /* If "DOWN" pushbutton is pressed */
    if(MyKey == DOWN)
    {
      /* Set the Text Color */
      LCD_SetTextColor(White);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 65, 85);
      if(i >= 6)
      {
        i -= 6;
      }
      else
      {
        i += 3;
        if(i == 8)
        {
          i = 7;
        }
      }
      /* Set the Text Color */
      LCD_SetTextColor(Blue);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 65, 85);

      /* Set the Back Color */
      LCD_SetBackColor(Blue);
      /* Set the Text Color */
      LCD_SetTextColor(White);
      /* Test on the MenuItemIndex value before incrementing it */
      if(MenuItemIndex >= 6)
      {
        MenuItemIndex -= 6;
      }
      else
      {
        MenuItemIndex += 3;
        if(MenuItemIndex == 8)
        {
          MenuItemIndex = 7;
        }
      }
      /* Get the current menu */
      psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
      LCD_DisplayStringLine(Line0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
    }
    /* If "LEFT" pushbutton is pressed */
    if(MyKey == LEFT)
    {
      /* Set the Text Color */
      LCD_SetTextColor(White);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 65, 85);
      if(i == 0)
      {
        i = 7;
      }
      else
      {
        i--;
      }
      /* Set the Text Color */
      LCD_SetTextColor(Blue);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 65, 85);

      /* Set the Back Color */
      LCD_SetBackColor(Blue);
      /* Set the Text Color */
      LCD_SetTextColor(White);
      if(MenuItemIndex > 0)
      {
        MenuItemIndex--;
      }
      else
      {
        MenuItemIndex = psCurrentMenu->nItems - 1;
      }

      psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
      LCD_DisplayStringLine(Line0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
    }
    /* If "RIGHT" pushbutton is pressed */
    if(MyKey == RIGHT)
    {
      /* Set the Text Color */
      LCD_SetTextColor(White);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 65, 85);
      if(i == 7)
      {
        i = 0x00;
      }
      else
      {
        i++;
      }
      /* Set the Text Color */
      LCD_SetTextColor(Blue);
      LCD_DrawRect(IconRect[i][0], IconRect[i][1], 65, 85);

      /* Set the Back Color */
      LCD_SetBackColor(Blue);
      /* Set the Text Color */
      LCD_SetTextColor(White);
      /* Test on the MenuItemIndex value before incrementing it */
      if(MenuItemIndex >= ((psCurrentMenu->nItems) - 1))
      {
        MenuItemIndex = 0;
      }
      else
      {
        MenuItemIndex++;
      }
      /* Get the current menu */
      psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
      LCD_DisplayStringLine(Line0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
    }
    /* If "SEL" pushbutton is pressed */
    if(MyKey == SEL)
    {
      SelFunc();
      IntExtOnOffConfig(ENABLE);
      return;
    }
  }    
}

/*******************************************************************************
* Function Name  : FunnyCircles
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FunnyCircles(void)
{
  uint32_t radius = 110, color = Red;
  uint32_t pressedkey = 0;

  /* Disable the JoyStick Interrupts */
  IntExtOnOffConfig(DISABLE);
  
  while(ReadKey() != NOKEY)
  {
  } 
  
  /* Set the Back Color */
  LCD_SetBackColor(White);
  
  /* Set the Text Color */
  LCD_SetTextColor(Red);

  LCD_Clear(Black);

  /* Draw a rectangle */
  LCD_DrawRect(0, 319, 239, 320);

  /* Check which key is pressed */
  pressedkey = ReadKey();  
      
  while(pressedkey == NOKEY)
  {
    /* Check which key is pressed */
    pressedkey = ReadKey();
    LCD_SetTextColor(color);
    color += 0x10; 
    LCD_DrawCircle(120, 160, radius);
    radius -= 1;
    if(radius == 0)
    {
      radius = 110;
    }
  }

  /* Clear the LCD */
  LCD_Clear(White);
  /* Display the previous menu */
  DisplayMenu();
  /* Enable the JoyStick interrupts */
  IntExtOnOffConfig(ENABLE);
}

/*******************************************************************************
* Function Name  : STM32Intro
* Description    : Display the STM32 introduction.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void STM32Intro(void)
{
  /* Set the Back Color */
  LCD_SetBackColor(White);
  /* Set the Text Color */
  LCD_SetTextColor(Blue);
  
  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  LCD_DrawBMP(0x0064FD00);
  Delay(100);
  
  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  LCD_DrawBMP(SlideAddr[0]);  
  Delay(100);  
}

/*******************************************************************************
* Function Name  : HelpFunc
* Description    : Display the Help menu.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HelpFunc(void)
{ 
  IntExtOnOffConfig(DISABLE);

  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  LCD_DrawBMP(0x00675600);

  while(ReadKey() == NOKEY)
  {
  }
  /* Set the Back Color */
  LCD_SetBackColor(Blue);
  /* Set the Text Color */
  LCD_SetTextColor(White);

  LCD_DisplayStringLine(Line0, "UP, DOWN, RIGHT and ");
  LCD_DisplayStringLine(Line1, "LEFT push-buttons   ");
  LCD_DisplayStringLine(Line2, "perform circular    ");
  LCD_DisplayStringLine(Line3, "navigation in the   ");
  LCD_DisplayStringLine(Line4, "main menu, current  ");
  LCD_DisplayStringLine(Line5, "menu items. SEL     ");
  LCD_DisplayStringLine(Line6, "push-button selects ");
  LCD_DisplayStringLine(Line7, "the current item. UP");
  LCD_DisplayStringLine(Line8, "and DOWN perform    ");
  LCD_DisplayStringLine(Line9, "vertical navigation.");
  
  while(ReadKey() == NOKEY)
  {
  }

  LCD_Clear(White);
  DisplayMenu();
  IntExtOnOffConfig(ENABLE);
}

/*******************************************************************************
* Function Name  : AboutFunc
* Description    : Display the About menu.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AboutFunc(void)
{
  LCD_Clear(White);

  IntExtOnOffConfig(DISABLE);

  while(ReadKey() != NOKEY)
  {
  } 
  /* Set the Back Color */
  LCD_SetBackColor(Blue);
  /* Set the Text Color */
  LCD_SetTextColor(White);

  LCD_ClearLine(Line0);
  LCD_DisplayStringLine(Line1, "   STM3210B-EVAL    ");
  LCD_DisplayStringLine(Line2, "   Demonstration    ");
  LCD_DisplayStringLine(Line3, "   Version 2.0.0    ");
  LCD_ClearLine(Line4);
  LCD_DisplayStringLine(Line5, "MCD Application Team");
  LCD_DisplayStringLine(Line6, "   COPYRIGHT 2009   ");
  LCD_DisplayStringLine(Line7, " STMicroelectronics ");
  LCD_ClearLine(Line8);
  LCD_DisplayStringLine(Line9, "  www.st.com/stm32  ");    

  while(ReadKey() == NOKEY)
  {
  }

  LCD_Clear(White);
  DisplayMenu();
  IntExtOnOffConfig(ENABLE);
}

/*******************************************************************************
* Function Name  : ProductPres
* Description    : Display the About menu.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ProductPres(void)
{
  __IO uint32_t index = 0, KeyState = 0;

  IntExtOnOffConfig(DISABLE);

  while(ReadKey() != NOKEY)
  {
  }

  if (Get_WaveFileStatus() == Valid_WAVE_File)
  {
    /* Start Wave audio playing */
    WavePlayer_Start();
  }
  else
  {
    LCD_DisplayStringLine(Line5, " No loaded wave file");
    LCD_DisplayStringLine(Line6, "  Press JoyStick to ");
    LCD_DisplayStringLine(Line7, "  continue...       ");
    while(ReadKey() == NOKEY)
    {
    }
    LCD_Clear(White);
    DisplayMenu();
    IntExtOnOffConfig(ENABLE);
    return;
  }
  LCD_Clear(White);
  /* Set the Back Color */
  LCD_SetBackColor(Blue);
  /* Set the Text Color */
  LCD_SetTextColor(White);
  
  LCD_ClearLine(Line0);
  LCD_DisplayStringLine(Line1, " Press SEL to start ");  
  LCD_ClearLine(Line2);
  LCD_ClearLine(Line3);
  LCD_DisplayStringLine(Line4, " When presentation  ");
  LCD_DisplayStringLine(Line5, "starts use RIGHT and"); 
  LCD_DisplayStringLine(Line6, "LEFT to go to the   ");
  LCD_DisplayStringLine(Line7, "next/previous slide ");
  LCD_DisplayStringLine(Line8, "and SEL to exit     ");
  LCD_ClearLine(Line9);

  while(ReadKey() != SEL)
  {
  }
  while(ReadKey() != NOKEY)
  {
  }
  /* Enable the SysTick Counter */
   SysTick->CTRL |= SysTick_CTRL_ENABLE;
  
  while(index < 14)
  {
    WavePlayer_Pause();
    LCD_SetDisplayWindow(239, 0x13F, 240, 320);

    /* Slide n°: index */
    LCD_DrawBMP(SlideAddr[index]);
   
    WavePlayer_RePlay();
    
    /* Clear the SysTick Counter */
    SysTick->VAL = (uint32_t)0x0;
    
    KeyState = DelayJoyStick(DelayTiming[index]);

    if(KeyState == LEFT)
    {
      if(index == 0)
      {
        index = 1;
      }
      index--;
      Set_WaveDataLength(SlidesSpeech[index]);
    }
    else if (KeyState == RIGHT)
    {
      if(index == 13)
      { 
        index = 15;
      }
      index++;
      Set_WaveDataLength(SlidesSpeech[index]);
    }
    else if (KeyState == SEL)
    {
      WavePlayer_Stop();
      index = 15;
    }
    else if (KeyState == NOKEY)
    {
      index++;
    }
  }
  /* Disable the SysTick Counter */
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE;

  /* Clear the SysTick Counter */
  SysTick->VAL = (uint32_t)0x0;
  
  LCD_Clear(Black);

  /* Set the Back Color */
  LCD_SetBackColor(Black);

  /* Set the Text Color */
  LCD_SetTextColor(White);

  LCD_DisplayStringLine(Line0, "  End of slide show ");
  LCD_DisplayStringLine(Line1, "Exit:  Push JoyStick");

  while(ReadKey() == NOKEY)
  {
  }

  LCD_Clear(White);
  DisplayMenu();
  IntExtOnOffConfig(ENABLE);
}

/*******************************************************************************
* Function Name  : CheckBitmapFiles
* Description    : Checks if the bitmapfiles (slides + icons) are already loaded
*                  in the SPI FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t CheckBitmapFiles(void)
{
  uint32_t index = 0;
  uint8_t Tab[6];
  uint32_t Icons128Addr[3] = {0x00637A00, 0x0063FB00, 0x00647C00};

  Tab[0] = 0x00;
  Tab[1] = 0x00;
  Tab[2] = 0x00;
  Tab[3] = 0x00;
  Tab[4] = 0x00;
  Tab[5] = 0x00;

  for(index = 0; index < 14; index++)
  {
    /* Read bitmap size */
    SPI_FLASH_BufferRead(Tab, SlideAddr[index], 6);
    if(Buffercmp(Tab, SlidesCheck, 5)!= 0) return 1;
    Tab[0] = 0x00;
    Tab[1] = 0x00;
    Tab[2] = 0x00;
    Tab[3] = 0x00;
    Tab[4] = 0x00;
    Tab[5] = 0x00;
  }
  /* Read bitmap size */
  SPI_FLASH_BufferRead(Tab, 0x0064FD00, 6);
  if(Buffercmp(Tab, SlidesCheck, 5)!= 0) return 1;
  Tab[0] = 0x00;
  Tab[1] = 0x00;
  Tab[2] = 0x00;
  Tab[3] = 0x00;
  Tab[4] = 0x00;
  Tab[5] = 0x00;
  /* Read bitmap size */
  SPI_FLASH_BufferRead(Tab, 0x00675600, 6);
  if(Buffercmp(Tab, SlidesCheck, 5)!= 0) return 1;
  Tab[0] = 0x00;
  Tab[1] = 0x00;
  Tab[2] = 0x00;
  Tab[3] = 0x00;
  Tab[4] = 0x00;
  Tab[5] = 0x00;

  for(index = 0; index < 8; index++)
  {
    /* Read bitmap size */
    SPI_FLASH_BufferRead(Tab, IconsAddr[index], 6);
    if(Buffercmp(Tab, Icons64Check, 5)!= 0) return 1;
    Tab[0] = 0x00;
    Tab[1] = 0x00;
    Tab[2] = 0x00;
    Tab[3] = 0x00;
    Tab[4] = 0x00;
    Tab[5] = 0x00;
  }

  for(index = 0; index < 3; index++)
  {
    /* Read bitmap size */
    SPI_FLASH_BufferRead(Tab, Icons128Addr[index], 6);
    if(Buffercmp(Tab, Icons128Check, 5)!= 0) return 1;
    Tab[0] = 0x00;
    Tab[1] = 0x00;
    Tab[2] = 0x00;
    Tab[3] = 0x00;
    Tab[4] = 0x00;
    Tab[5] = 0x00;
  }
  return 0;
}

/*******************************************************************************
* Function Name  : Buffercmp8
* Description    : Compares two buffers.
* Input          : - pBuffer1, pBuffer2: buffers to be compared.
*                : - BufferLength: buffer's length
* Output         : None
* Return         : 0: pBuffer1 identical to pBuffer2
*                  1: pBuffer1 differs from pBuffer2
*******************************************************************************/
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
