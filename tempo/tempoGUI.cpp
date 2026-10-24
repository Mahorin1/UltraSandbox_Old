/*
function GUI.NewText(pX,pY,pLarg,pLong,pText,pFont,pXalign,pYalign)
  local MyText=NewElement(pX,pY)
  MyText.larg,MyText.long=pLarg,pLong
  MyText.largText,MyText.longText=pFont:getWidth(pText),pFont:getHeight(pText)
  MyText.Text=pText
  MyText.Font=pFont
  MyText.Xalign,MyText.Yalign=pXalign,pYalign
  
  function MyText:SetDimension(pLarg,pLong)
    self.larg,self.long=pLarg,pLong
  end
  
  function MyText:SetText(pText)
    self.largText,self.longText=self.Font:getWidth(pText),self.Font:getHeight(pText)
    self.Text=pText
  end
  
  function MyText:drawText()
    love.graphics.setColor(self.rgba[1],self.rgba[2],self.rgba[3],self.rgba[4])
    love.graphics.setFont(self.Font)
    local x,y=self.x,self.y
    if pXalign=="center" then
      x=x+((self.larg-self.largText)/2)
    end
    if pYalign == "center" then
      y=y+((self.long-self.longText)/2)
    end
    love.graphics.print(self.Text,x,y)
  end
  
  function MyText:draw()
    if self.Visible==true then self:drawText() end
  end
  
  return MyText
end



function GUI.NewProgressBar(pX,pY,pLarg,pLong,pMax,pType)
  local myProgressBar = GCGUI.newPanel(pX,pY,pLarg,pLong) pType=pType or "horizontal"
  myProgressBar.Max=pMax
  myProgressBar.Value=pMax
  myProgressBar.imgBack=nil
  myProgressBar.imgBar=nil
  myProgressBar.type=pType
  myProgressBar.BackRgba=nil
  function myProgressBar:setImages(pImageBack, pImageBar)
    self.imgBack=pImageBack
    self.imgBar=pImageBar
    self.larg=pImageBack:getWidth()
    self.long=pImageBack:getHeight()
  end
  function myProgressBar:setValue(pValue) if pValue>=0 and pValue<=self.Max then self.Value=pValue end end
  function myProgressBar:draw()
    love.graphics.setColor(self.rgba[1],self.rgba[2],self.rgba[3],self.rgba[4])
    local barSize,ajust=self.Value/self.Max,0
    if self.imgBack~=nil and self.imgBar~=nil then
      love.graphics.draw(self.imgBack,self.x,self.y) local barQuad
      if self.type=="horizontal" then
        barQuad=love.graphics.newQuad(0,0,self.larg*barSize,self.long,self.larg,self.long)
      else 
        barQuad=love.graphics.newQuad(0,self.long,self.larg,-self.long*barSize,self.larg,self.long) ajust=self.long
      end
      love.graphics.draw(self.imgBar,barQuad,self.x,self.y+ajust)
    else
      if self.BackRgba~=nil then love.graphics.setColor(self.BackRgba[1],self.BackRgba[2],self.BackRgba[3],self.BackRgba[4]) else love.graphics.setColor(0,0,0) end self:drawPanel()
      love.graphics.setColor(self.rgba[1],self.rgba[2],self.rgba[3],self.rgba[4])
      local ecart=0.85 if self.TypeDraw=="fill" then ecart=0 end
      if self.type=="horizontal" then
        love.graphics.rectangle("fill", self.x+ecart, self.y+ecart,(self.larg-ecart*2)*barSize,self.long-ecart*2)
      else
        love.graphics.rectangle("fill", self.x+ecart, self.y+ecart+self.long,self.larg-ecart*2,(-self.long-ecart*2)*barSize)
      end
    end
  end
  return myProgressBar
end*/