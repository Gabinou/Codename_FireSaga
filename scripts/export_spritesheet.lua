if TilesetMode == nil then return app.alert "Use Aseprite v1.3"  end

-- Export spritesheet for aseprite files while ignoring two layers: Background and Shadow

local fs = app.fs
local spr = app.activeSprite
local output_folder = fs.filePath(spr.filename)
local path = fs.joinPath(output_folder, app.fs.fileTitle(spr.filename) .. ".png")

app.command.ExportSpriteSheet {
  ui=false,
  askOverwrite=false,
  type=SpriteSheetType.HORIZONTAL,
  columns=8,
  rows=0,
  width=0,
  height=0,
  bestFit=false,
  textureFilename=path,
  dataFilename="",
  dataFormat=SpriteSheetDataFormat.JSON_HASH,
  borderPadding=0,
  shapePadding=0,
  innerPadding=0,
  trim=false,
  extrude=false,
  openGenerated=false,
  layer="Guard",
  tag="",
  splitLayers=false,
  listLayers=true,
  listTags=true,
  listSlices=true
}

