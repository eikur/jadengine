#include "Globals.h"
#include "Application.h"
#include "ModuleFonts.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

ModuleFonts::ModuleFonts()
{
}

ModuleFonts::~ModuleFonts()
{
}

bool ModuleFonts::Init()
{
	bool res = true;
	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		MYLOG("Fonts: Unable to load from config file\n");
		res = false;
	}
	else
	{
//		graphics = App->textures->Load(asset_file.c_str());
	}
	return res;
}

bool ModuleFonts::CleanUp()
{
	MYLOG("Fonts: Unloading fonts\n");

//	App->textures->Unload(graphics);

	for (std::vector<Font*>::iterator it = fonts.begin(); it != fonts.end(); ++it)
		RELEASE(*it);
	fonts.clear();
	
	return true;

}

bool ModuleFonts::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value = json_parse_file(file_path);
	JSON_Array *j_rects;
	JSON_Array *j_rect;
	JSON_Array *j_lookup;
	JSON_Array *j_ppe;

	if (root_value == nullptr)
		return false;

	int total = (int) json_object_dotget_number(json_object(root_value), "fonts.total");
	if (total == 0) {
		json_value_free(root_value);
		return false;
	}

	//get the path to the asset
	if (json_object_dothas_value_of_type(json_object(root_value), "fonts.file", JSONString))
		asset_file = json_object_dotget_string(json_object(root_value), "fonts.file");
	if (asset_file == "") {
		json_value_free(root_value);
		return false;
	}

	//get the fonts 
	j_rects = json_object_dotget_array(json_object(root_value), "fonts.rects");
	j_lookup = json_object_dotget_array(json_object(root_value), "fonts.lookup_table");
	j_ppe = json_object_dotget_array(json_object(root_value), "fonts.ppe");

	Font *tmp = new Font();

	for (int i = 0; i < total; ++i)
	{
		tmp->id = i;

		j_rect = json_array_get_array(j_rects, i);
		tmp->rect->x = (int)json_array_get_number(j_rect, 0);
		tmp->rect->y = (int)json_array_get_number(j_rect, 1);
		tmp->rect->w = (int)json_array_get_number(j_rect, 2);
		tmp->rect->h = (int)json_array_get_number(j_rect, 3);

		tmp->lookup_table = json_array_get_string(j_lookup, i);

		tmp->pixels_per_element = (int)json_array_get_number(j_ppe, i);

		fonts.push_back(new Font(*tmp));
	}

	delete tmp;

	json_value_free(root_value);

	return true;
}

void ModuleFonts::Print(int, int, int, const std::string ) const
{	
	// find selected font
/*	std::vector<Font*>::const_iterator it;

	for (it = fonts.cbegin(); it != fonts.cend(); ++it)
		if ((*it)->id == font_id)
			break;

	// draw the fonts
	int i = 0;
	int i_max = text.length();
	int lut_i;

	SDL_Rect *i_rect = new SDL_Rect();
	i_rect->y = (*it)->rect->y;
	i_rect->w = (*it)->pixels_per_element;
	i_rect->h = (*it)->rect->h;
	
	for (i = 0; i < i_max; ++i)
	{
		lut_i = (*it)->lookup_table.find(text.at(i));
		i_rect->x = (*it)->rect->x + lut_i * (*it)->pixels_per_element;
		App->renderer->Blit(graphics, x+i*(*it)->pixels_per_element, y, i_rect, 0.0f);
	}

	delete i_rect;
*/
}

std::string ModuleFonts::GetPrintableValue(int value, int desired_length) const
{
	std::string ret = "";
	std::string tmp = std::to_string(value);

	int current_length = tmp.length();

	for (int i = current_length; i < desired_length; ++i)
	{
		ret.append("0");
	}
	ret.append(tmp);
	return ret;
}

/*---------------------------------------------------------*/

Font::Font()
{
	rect = new SDL_Rect();
	id = 0;
	lookup_table = "";
	pixels_per_element = 0;
}

Font::Font(const Font& f)
{
	rect = new SDL_Rect(*f.rect);
	id = f.id;
	lookup_table = f.lookup_table;
	pixels_per_element = f.pixels_per_element;
}

Font::~Font()
{
	delete rect;
}